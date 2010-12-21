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
  [super showWindow:sender];
}

- (IBAction)connect:(id)sender {
	[self.window orderOut:self];
  [network connect_to:[address stringValue] withPort:SERVER_PORT];
  [status_menu add_recent_item:[address stringValue]];
}

@end
