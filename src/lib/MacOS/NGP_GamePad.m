#ifdef __OBJC__
#import <AppKit/AppKit.h>
#import <GameController/GameController.h>
#include <NGP_GamePad.h>

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
  usleep(5000); // TODO figure out how to get a notification that the
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

struct NGP_GamePad{
  GCController *controller;

};

NGP_GamePad* NGP_GamePadOpen(int index) {
  NGP_GamePad* gp = malloc(sizeof(NGP_GamePad));
  gp->controller = GCController.controllers[(NSUInteger)index];
  [gp->controller retain];
  return gp;
}

void NGP_GamePadFree(NGP_GamePad* gp) {
  [gp->controller release];
  free(gp);
}

const char* NGP_GamePadName(NGP_GamePad* gp) {
  return gp->controller.physicalInputProfile.device.productCategory.UTF8String;
}

#endif