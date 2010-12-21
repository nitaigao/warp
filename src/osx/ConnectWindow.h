#import <Cocoa/Cocoa.h>
#import "Network.h"
#import "StatusMenu.h"

@interface ConnectWindow : NSWindowController {
  IBOutlet Network *network;
  IBOutlet NSTextField *address;
  IBOutlet NSWindowController *bezel_window;
  IBOutlet StatusMenu *status_menu;
}

- (IBAction)connect:(id)sender;

@end
