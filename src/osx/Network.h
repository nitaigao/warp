#import <Cocoa/Cocoa.h>

#import "Entrance.h"
#import "BezelWindow.h"

@interface Network : NSObject {
  IBOutlet Entrance* entrance;
  IBOutlet BezelWindow* bezel_window;
  
  bool quit;
}

- (void)on_event:(CGEventType)eventType withEvent:(CGEventRef)event;
- (void)connect_to:(NSString*)address withPort:(unsigned int)port;
- (void)toggle;
- (bool)is_connected;
- (bool)understands:(CGEventType)eventType;

- (void)stop;
- (void)exit_thread;

@end
