#import <Cocoa/Cocoa.h>

#import "Entrance.h"
#import "BezelWindow.h"
#import "StatusMenu.h"
#import "Multicast.h"

@interface Network : NSObject {
  IBOutlet Entrance* entrance;
  IBOutlet BezelWindow* bezel_window;
  IBOutlet StatusMenu* status_menu;
  
  Multicast multicast;
  
  bool quit;
}

- (void)on_event:(CGEventType)eventType withEvent:(CGEventRef)event;
- (void)connect_to:(NSString*)address withPort:(unsigned int)port;
- (void)toggle;
- (bool)is_connected;
- (bool)understands:(CGEventType)eventType;

- (void)quit;
- (void)recent:(NSString*)address;

- (void)exit_thread;
- (void)broadcast_thread;
- (void)subscriber_thread;
- (void)update_thread;
@end
