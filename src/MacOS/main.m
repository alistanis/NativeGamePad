
#ifdef __OBJC__
#import <AppKit/AppKit.h>
#import <GameController/GameController.h>

@interface AppDelegate : NSObject<NSApplicationDelegate> @end

@implementation AppDelegate
- (void) applicationDidFinishLaunching: (NSNotification*) notification {
    NSEvent* dummyEvent = [NSEvent
            otherEventWithType: NSEventTypeApplicationDefined
                      location: NSZeroPoint
                 modifierFlags: 0
                     timestamp: 0
                  windowNumber: 0
                       context: nil
                       subtype:0
                         data1:0
                         data2:0];
    [NSApp postEvent: dummyEvent atStart: TRUE];
    [NSApp stop: nil]; // Allows [app run] to return
}
@end

void initialize_controller_framework(void);

void initialize_controller_framework(void) {
    NSApplication* app = [NSApplication sharedApplication];
    [app setActivationPolicy: NSApplicationActivationPolicyAccessory];
    [app setDelegate: [[AppDelegate alloc] init]];
    [app run];

    // 1 with a DualShock 4 plugged in
    printf("controllers %lu\n", [[GCController controllers] count]);

    for (GCController* c in [GCController controllers]) {
        NSLog(@"%@", c.debugDescription);
        NSLog(@"%@", c.extendedGamepad.description);
        NSLog(@"%@", c.extendedGamepad.debugDescription);
        NSLog(@"%@", c.extendedGamepad.class);

        NSLog(@"%@", c.haptics.debugDescription);
        CHHapticEngine* h = [c.haptics createEngineWithLocality: GCHapticsLocalityDefault];
        NSLog(@"%@", h);
        if (@available(macOS 11.3, *)) {
            if ([c.extendedGamepad isKindOfClass:[GCDualSenseGamepad class]]) {
            NSLog(@"We've found a dual sense gamepad!");
                GCDualSenseGamepad *ds = (GCDualSenseGamepad *)c.extendedGamepad;
              NSLog(@"%@", ds.debugDescription);
              NSLog(@"%@", c.extendedGamepad);
              NSLog(@"");
            }
        } else {
            // Fallback on earlier versions
        }
    }

}

int main() {
    initialize_controller_framework();
    // Do stuff here
    return 0;
}

#endif