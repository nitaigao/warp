#import <Cocoa/Cocoa.h>
#import "Network.h"
#import "StatusMenu.h"

@interface ConnectWindow : NSWindowController {
  IBOutlet Network *network;
  IBOutlet NSTextField *address;
}

- (IBAction)connect:(id)sender;

@end
