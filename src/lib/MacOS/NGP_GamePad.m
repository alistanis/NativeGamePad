#ifdef __OBJC__
#import <AppKit/AppKit.h>
#import <GameController/GameController.h>
#import <IOKit/hid/IOHIDLib.h>

#include <NGP_GamePad.h>

CFStringRef NGP_DARWIN_RUN_LOOP = CFSTR("NGP_DARWIN_RUN_LOOP");
#define NGP_HARDWARE_BUS_USB        0x03
#define NGP_HARDWARE_BUS_BLUETOOTH  0x05

typedef struct {
  uint8_t data[16];
} NGP_DeviceGUID;

struct NGP_IODevice{
  IOHIDDeviceRef deviceRef; /* HIDManager device handle */

  char product[256]; /* name of product */
  char manufacturer[256];
  uint32_t usage; /* usage page from IOUSBHID Parser.h which defines general usage */
  uint32_t
      usagePage; /* usage within above page from IOUSBHID Parser.h which defines specific usage */
  NGP_DeviceGUID guid;

  int instance_id;
  int32_t vendor_id;
  int32_t product_id;
  int32_t version;
  bool removed;
  bool runLoopAttached; /* is 'deviceRef' attached to a CFRunLoop? */
};
typedef struct NGP_IODevice NGP_IODevice;

struct NGP_DeviceContext{
  NGP_IODevice* device;
  struct NGP_DeviceContext* next;
};

typedef struct NGP_DeviceContext NGP_DeviceContext;

typedef struct {
  NGP_DeviceContext* ctx;
  int current_index;
} NGP_DeviceContextManager;


static void
FreeDevice(NGP_IODevice *removeDevice)
{
  if (removeDevice) {
    if (removeDevice->deviceRef) {
      if (removeDevice->runLoopAttached) {
        /* Calling IOHIDDeviceUnscheduleFromRunLoop without a prior,
         * paired call to IOHIDDeviceScheduleWithRunLoop can lead
         * to crashes in MacOS 10.14.x and earlier.  This doesn't
         * appear to be a problem in MacOS 10.15.x, but we'll
         * do it anyways.  (Part-of fix for Bug 5034)
         */
        IOHIDDeviceUnscheduleFromRunLoop(removeDevice->deviceRef, CFRunLoopGetCurrent(), NGP_DARWIN_RUN_LOOP);
      }
      CFRelease(removeDevice->deviceRef);
      removeDevice->deviceRef = NULL;
    }

    free(removeDevice);
  }
}


static CFDictionaryRef CreateHIDDeviceMatchDictionary(const UInt32 page,
                                                      const UInt32 usage,
                                                      int* okay) {
  CFDictionaryRef ref = NULL;
  CFNumberRef pageNumRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &page);
  CFNumberRef usageNumRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usage);
  const void* keys[2] = {(void*)CFSTR(kIOHIDDeviceUsagePageKey),
                         (void*)CFSTR(kIOHIDDeviceUsageKey)};
  const void* vals[2] = {(void*)pageNumRef, (void*)usageNumRef};

  if (pageNumRef && usageNumRef) {
    ref = CFDictionaryCreate(kCFAllocatorDefault, keys, vals, 2, &kCFTypeDictionaryKeyCallBacks,
                             &kCFTypeDictionaryValueCallBacks);
  }

  if (pageNumRef) {
    CFRelease(pageNumRef);
  }
  if (usageNumRef) {
    CFRelease(usageNumRef);
  }

  if (!ref) {
    *okay = 0;
  }
  *okay = 1;
  return ref;
}

static bool GetDeviceInfo(IOHIDDeviceRef hidDevice, NGP_IODevice* device) {
  int32_t vendor = 0;
  int32_t product = 0;
  int32_t version = 0;
  char manufacturer_string[256];
  char product_string[256];
  CFTypeRef refCF = NULL;
  uint16_t* guid16 = (uint16_t*)device->guid.data;

  /* get usage page and usage */
  refCF = IOHIDDeviceGetProperty(hidDevice, CFSTR(kIOHIDPrimaryUsagePageKey));
  if (refCF) {
    CFNumberGetValue(refCF, kCFNumberSInt32Type, &device->usagePage);
  }
  if (device->usagePage != kHIDPage_GenericDesktop) {
    return false; /* Filter device list to non-keyboard/mouse stuff */
  }

  refCF = IOHIDDeviceGetProperty(hidDevice, CFSTR(kIOHIDPrimaryUsageKey));
  if (refCF) {
    CFNumberGetValue(refCF, kCFNumberSInt32Type, &device->usage);
  }

  if (device->usage != kHIDUsage_GD_GamePad) {
    return false; /* Filter device list to non-keyboard/mouse stuff */
  }

  /* Make sure we retain the use of the IOKit-provided device-object,
     lest the device get disconnected and we try to use it.  (Fixes
     SDL-Bugzilla #4961, aka. https://bugzilla.libsdl.org/show_bug.cgi?id=4961 )
  */
  CFRetain(hidDevice);

  /* Now that we've CFRetain'ed the device-object (for our use), we'll
     save the reference to it.
  */
  device->deviceRef = hidDevice;

  refCF = IOHIDDeviceGetProperty(hidDevice, CFSTR(kIOHIDVendorIDKey));
  if (refCF) {
    CFNumberGetValue(refCF, kCFNumberSInt32Type, &vendor);
  }

  refCF = IOHIDDeviceGetProperty(hidDevice, CFSTR(kIOHIDProductIDKey));
  if (refCF) {
    CFNumberGetValue(refCF, kCFNumberSInt32Type, &product);
  }

  refCF = IOHIDDeviceGetProperty(hidDevice, CFSTR(kIOHIDVersionNumberKey));
  if (refCF) {
    CFNumberGetValue(refCF, kCFNumberSInt32Type, &version);
  }

  /* get device name */
  refCF = IOHIDDeviceGetProperty(hidDevice, CFSTR(kIOHIDManufacturerKey));
  if ((!refCF) || (!CFStringGetCString(refCF, manufacturer_string, sizeof(manufacturer_string),
                                       kCFStringEncodingUTF8))) {
    manufacturer_string[0] = '\0';
  }
  refCF = IOHIDDeviceGetProperty(hidDevice, CFSTR(kIOHIDProductKey));
  if ((!refCF) ||
      (!CFStringGetCString(refCF, product_string, sizeof(product_string), kCFStringEncodingUTF8))) {
    product_string[0] = '\0';
  }
  strlcpy(device->manufacturer, manufacturer_string, sizeof(device->manufacturer));
  strlcpy(device->product, product_string, sizeof(device->product));

  memset(device->guid.data, 0, sizeof(device->guid.data));

  if (vendor && product) {
    *guid16++ = NGP_SwapLE16(NGP_HARDWARE_BUS_USB);
    *guid16++ = 0;
    *guid16++ = NGP_SwapLE16((uint16_t)vendor);
    *guid16++ = 0;
    *guid16++ = NGP_SwapLE16((uint16_t)product);
    *guid16++ = 0;
    *guid16++ = NGP_SwapLE16((uint16_t)version);
    *guid16++ = 0;
  } else {
    *guid16++ = NGP_SwapLE16(NGP_HARDWARE_BUS_BLUETOOTH);
    *guid16++ = 0;
    strlcpy((char*)guid16, device->product, sizeof(device->guid.data) - 4);
  }

  return true;
}


static void GamePadDeviceWasRemovedCallback(void* ctx,
                                          IOReturn res,
                                          void* sender) {
  printf("Device was removed");
  NGP_DeviceContext *dev_ctx = (NGP_DeviceContext*)(ctx);
  FreeDevice(dev_ctx->device);
  free(dev_ctx);

}

static void GamePadDeviceWasAddedCallback(void* ctx,
                                          IOReturn res,
                                          void* sender,
                                          IOHIDDeviceRef ioHIDDeviceObject) {
  if (res != kIOReturnSuccess) {
    printf("Device return was not successful, was %d", res);
    return;
  }

  NGP_DeviceContextManager *manager = (NGP_DeviceContextManager *)(ctx);
  int index = manager->current_index;
  manager->current_index++;

  NGP_IODevice *device;
  int device_index = dev_ctx->current_index;
  dev_ctx->current_index++;
  device = (NGP_IODevice *) calloc(1, sizeof(NGP_IODevice));
  assert(device);

  if (!GetDeviceInfo(ioHIDDeviceObject, device)) {
    FreeDevice(device);
    return;   /* not a device we care about, probably. */
  }
  dev_ctx->device = device;
  /* Get notified when this device is disconnected. */
  IOHIDDeviceRegisterRemovalCallback(ioHIDDeviceObject, GamePadDeviceWasRemovedCallback, dev_ctx);
  IOHIDDeviceScheduleWithRunLoop(ioHIDDeviceObject, CFRunLoopGetCurrent(), NGP_DARWIN_RUN_LOOP);
  device->runLoopAttached = true;

  /* Allocate an instance ID for this device */
  device->instance_id = device_index;

}

static bool ConfigureHIDManager(IOHIDManagerRef hidman, CFDictionaryRef matchingDict, NGP_DeviceContextManager* context_manager) {
  CFRunLoopRef runloop = CFRunLoopGetCurrent();

  if (IOHIDManagerOpen(hidman, kIOHIDOptionsTypeNone) != kIOReturnSuccess) {
    return false;
  }

  IOHIDManagerSetDeviceMatching(hidman, matchingDict);
  IOHIDManagerRegisterDeviceMatchingCallback(hidman, GamePadDeviceWasAddedCallback, context_manager);
  IOHIDManagerScheduleWithRunLoop(hidman, runloop, NGP_DARWIN_RUN_LOOP);

  while (CFRunLoopRunInMode(NGP_DARWIN_RUN_LOOP, 0, TRUE) == kCFRunLoopRunHandledSource) {
    /* no-op. Callback fires once per existing device. */
  }

  return true; /* good to go. */
}

static IOHIDManagerRef CreateHIDManager(NGP_DeviceContextManager* context_manager) {

  IOHIDManagerRef ref = NULL;
  ;
  int ok = 0;

  CFDictionaryRef d =
      CreateHIDDeviceMatchDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_GamePad, &ok);
  if (d) {
    ref = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    if (ref != NULL) {
      ok = ConfigureHIDManager(ref, d, context_manager);
    }
    CFRelease(d);
  }
  if (ok) {
    return ref;
  }
  return NULL;
}

@interface AppDelegate : NSObject <NSApplicationDelegate>
@end

@implementation AppDelegate
- (void)applicationDidFinishLaunching:(NSNotification*)notification {
  NSEvent* dummyEvent = [NSEvent otherEventWithType:NSEventTypeApplicationDefined
                                           location:NSZeroPoint
                                      modifierFlags:(NSEventModifierFlags)0
                                          timestamp:0
                                       windowNumber:0
                                            context:nil
                                            subtype:0
                                              data1:0
                                              data2:0];
  [NSApp postEvent:dummyEvent atStart:TRUE];
  usleep(5000);     // TODO figure out how to get a notification that the
                    // system has finished searching for connected game pads :(
  [NSApp stop:nil]; // Allows [app run] to return
}
@end

void NGP_Initialize(void) {
  NSApplication* app = [NSApplication sharedApplication];
  [app setActivationPolicy:NSApplicationActivationPolicyAccessory];
  [app setDelegate:[[[AppDelegate alloc] init] autorelease]];
  [app run];

  for (GCController* c in [GCController controllers]) {
    if (@available(macOS 11.3, *)) {
      if ([c.extendedGamepad isKindOfClass:[GCDualSenseGamepad class]]) {
        NSLog(@"We've found a dual sense gamepad!");
      }
    }
  }
}

int NGP_NumGamePads() {
  return (int)GCController.controllers.count;
}

struct NGP_GamePad {
  GCController* controller;
};

NGP_GamePad* NGP_GamePadOpen(int index) {
  NGP_GamePad* gp = malloc(sizeof(NGP_GamePad));
  gp->controller = CFRetain(GCController.controllers[(NSUInteger)index]);
  return gp;
}

void NGP_GamePadFree(NGP_GamePad* gp) {
  CFRelease(gp->controller);
  free(gp);
}

const char* NGP_GamePadName(NGP_GamePad* gp) {
  return gp->controller.productCategory.UTF8String;
}

bool NGP_GamePadIsAttached(NGP_GamePad* gp) {
  return gp->controller.attachedToDevice;
}

const char* NGP_GamePadSerial(NGP_GamePad* gp) {
  id device_id = gp->controller.physicalInputProfile.device;
}

#endif