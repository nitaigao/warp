//
//  ConnectWindow.m
//  warp
//
//  Created by Nicholas Kostelnik on 20/12/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "ConnectWindow.h"


@implementation ConnectWindow

- (IBAction)showWindow:(id)sender {
  [[NSApplication sharedApplication] activateIgnoringOtherApps : YES];
  [super showWindow:sender];
}

- (IBAction)connect:(id)sender {
  [[NSApplication sharedApplication] activateIgnoringOtherApps : YES];
	[self.window orderOut:self];
  [bezel_window showWindow:self];
  [network connect_to:[address stringValue] withPort:SERVER_PORT];
}

@end
