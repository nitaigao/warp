#import "ConnectWindow.h"

@implementation ConnectWindow

- (IBAction)showWindow:(id)sender {
  [super showWindow:sender];
  [[NSApplication sharedApplication] activateIgnoringOtherApps : YES];
}

- (IBAction)connect:(id)sender {
	[self.window orderOut:self];
  [network connect_to:[address stringValue] withPort:SERVER_PORT];
}

@end
