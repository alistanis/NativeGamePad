/*
Native Game Pad
Copyright (C) 2021 Christopher Cooper <christopher.michael.cooper@gmail.com>

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

The code below dealing with GamePads from MacOS is original.
The code below interacting with IOKit was taken and altered from SDL:

Simple DirectMedia Layer
Copyright (C) 1997-2020 Sam Lantinga <slouken@libsdl.org>

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifdef __OBJC__
#import <AppKit/AppKit.h>
#import <GameController/GameController.h>
#import <IOKit/hid/IOHIDLib.h>

#include <NGP_GamePad.h>
#include "CHSinglyLinkedList.h"

CFStringRef NGP_DARWIN_RUN_LOOP = CFSTR("NGP_DARWIN_RUN_LOOP");
#define NGP_HARDWARE_BUS_USB 0x03
#define NGP_HARDWARE_BUS_BLUETOOTH 0x05

typedef struct NGP_DeviceGUID {
  uint8_t data[16];
} NGP_DeviceGUID;

typedef struct NGP_IODevice {
  IOHIDDeviceRef deviceRef; /* HIDManager device handle */

  char product[256]; /* name of product */
  char manufacturer[256];
  uint64_t instance_id;

  uint32_t usage; /* usage page from IOUSBHID Parser.h which defines general usage */
  uint32_t
      usagePage; /* usage within above page from IOUSBHID Parser.h which defines specific usage */
  NGP_DeviceGUID guid;

  int32_t vendor_id;
  int32_t product_id;
  int32_t version;
  bool removed;
  bool runLoopAttached; /* is 'deviceRef' attached to a CFRunLoop? */
} NGP_IODevice;

static void PrintDevice(NGP_IODevice* device) {
  NSLog(@"%s", device->product);
  NSLog(@"%s", device->manufacturer);
  NSLog(@"%llu", device->instance_id);
  NSLog(@"%d", device->vendor_id);
  NSLog(@"%d", device->product_id);
  NSLog(@"%d", device->version);
}

static void FreeDevice(NGP_IODevice* removeDevice) {
  if (removeDevice) {
    if (removeDevice->deviceRef) {
      if (removeDevice->runLoopAttached) {
        /* Calling IOHIDDeviceUnscheduleFromRunLoop without a prior,
         * paired call to IOHIDDeviceScheduleWithRunLoop can lead
         * to crashes in MacOS 10.14.x and earlier.  This doesn't
         * appear to be a problem in MacOS 10.15.x, but we'll
         * do it anyways.  (Part-of fix for Bug 5034)
         */
        IOHIDDeviceUnscheduleFromRunLoop(removeDevice->deviceRef, CFRunLoopGetCurrent(),
                                         NGP_DARWIN_RUN_LOOP);
      }
      CFRelease(removeDevice->deviceRef);
      removeDevice->deviceRef = NULL;
    }

    free(removeDevice);
  }
}

typedef struct NGP_DeviceContextManager {
  CHSinglyLinkedList* device_list;
  uint64_t id_counter;
} NGP_DeviceContextManager;

typedef struct NGP_DeviceContext {
  NGP_DeviceContextManager* manager;
  id device_id;
} NGP_DeviceContext;

static CHSinglyLinkedList* NewSinglyLinkedList () {
  return CFRetain(CHSinglyLinkedList.alloc.init);
}

static void DeviceContextManagerFreeList(NGP_DeviceContextManager* manager) {
  CHSinglyLinkedList* l = manager->device_list;
  NSEnumerator* e = [l objectEnumerator];
  NSValue* c;
  c = [e nextObject];
  while (c) {
    FreeDevice([(c)pointerValue]);
    CFRelease(c);
    c = [e nextObject];
  }
  [l removeAllObjects];
  CFRelease(l);
}

#define NewDeviceContextManager() {NewSinglyLinkedList(), 0}

static id DeviceContextManagerInsert(NGP_DeviceContextManager* manager, NGP_IODevice* device) {
  device->instance_id = manager->id_counter;
  id val = CFRetain([NSValue valueWithPointer: device]);
  [manager->device_list addObject:(val)];
  manager->id_counter++;
  return val;
}

NSUInteger DeviceContextManagerIndexForObject(NGP_DeviceContextManager* manager, id val) {
  return [manager->device_list indexOfObject: val];
}

NGP_IODevice* DeviceContextManagerRemove(NGP_DeviceContextManager* manager, id val) {
  NSValue* value;
  NSUInteger index = [manager->device_list indexOfObject: val];
  value = [manager->device_list objectAtIndex: index];
  NGP_IODevice* device = [value pointerValue];
  [manager->device_list removeObject: val];
  return device;
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
  device->vendor_id = vendor;

  refCF = IOHIDDeviceGetProperty(hidDevice, CFSTR(kIOHIDProductIDKey));
  if (refCF) {
    CFNumberGetValue(refCF, kCFNumberSInt32Type, &product);
  }
  device->product_id = product;

  refCF = IOHIDDeviceGetProperty(hidDevice, CFSTR(kIOHIDVersionNumberKey));
  if (refCF) {
    CFNumberGetValue(refCF, kCFNumberSInt32Type, &version);
  }
  device->version = version;

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

static void GamePadDeviceWasRemovedCallback(void* ctx, IOReturn res, void* sender) {
  printf("Device was removed\n");
  NGP_DeviceContext* dev_ctx = (NGP_DeviceContext*)(ctx);
  NGP_IODevice* device = DeviceContextManagerRemove(dev_ctx->manager, dev_ctx->device_id);
  PrintDevice(device);
  FreeDevice(device);
  free(dev_ctx);
}

static void GamePadDeviceWasAddedCallback(void* ctx,
                                          IOReturn res,
                                          void* sender,
                                          IOHIDDeviceRef ioHIDDeviceObject) {
  printf("Device was added\n");
  if (res != kIOReturnSuccess) {
    printf("Device return was not successful, was %d", res);
    return;
  }

  NGP_DeviceContextManager* manager = (NGP_DeviceContextManager*)(ctx);

  NGP_IODevice* device;
  device = (NGP_IODevice*)calloc(1, sizeof(NGP_IODevice));
  assert(device);

  if (!GetDeviceInfo(ioHIDDeviceObject, device)) {
    FreeDevice(device);
    return; /* not a device we care about, probably. */
  }

  id val = DeviceContextManagerInsert(manager, device);

  NGP_DeviceContext* dev_ctx = calloc(1, sizeof(NGP_DeviceContext));
  assert(dev_ctx);

  dev_ctx->device_id = val;
  dev_ctx->manager = manager;

  /* Get notified when this device is disconnected. */
  IOHIDDeviceRegisterRemovalCallback(ioHIDDeviceObject, GamePadDeviceWasRemovedCallback, dev_ctx);
  IOHIDDeviceScheduleWithRunLoop(ioHIDDeviceObject, CFRunLoopGetCurrent(), NGP_DARWIN_RUN_LOOP);
  while (CFRunLoopRunInMode(NGP_DARWIN_RUN_LOOP, 0, TRUE) == kCFRunLoopRunHandledSource) {
    /* no-op. Callback fires once per existing device. */
  }

  device->runLoopAttached = true;
  PrintDevice(device);
}

static bool ConfigureHIDManager(IOHIDManagerRef hidman,
                                CFDictionaryRef matchingDict,
                                NGP_DeviceContextManager* context_manager) {
  CFRunLoopRef runloop = CFRunLoopGetCurrent();

  if (IOHIDManagerOpen(hidman, kIOHIDOptionsTypeNone) != kIOReturnSuccess) {
    return false;
  }

  IOHIDManagerSetDeviceMatching(hidman, matchingDict);
  IOHIDManagerRegisterDeviceMatchingCallback(hidman, GamePadDeviceWasAddedCallback,
                                             context_manager);
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

  NGP_DeviceContextManager manager = NewDeviceContextManager();
  IOHIDManagerRef hid_manager = CreateHIDManager(&manager);
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

  for(;;) {
    while (CFRunLoopRunInMode(NGP_DARWIN_RUN_LOOP, 0, TRUE) == kCFRunLoopRunHandledSource) {
      /* no-op. Callback fires once per existing device. */
    }
  }

  IOHIDManagerClose(hid_manager, kIOHIDOptionsTypeNone);
}

int NGP_NumGamePads() {
  return (int)GCController.controllers.count;
}

struct NGP_GamePad {
  GCController* controller;
  NGP_IODevice* device;
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