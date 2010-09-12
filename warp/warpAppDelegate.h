//
//  warpAppDelegate.h
//  warp
//
//  Created by Nicholas Kostelnik on 11/09/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include <ApplicationServices/ApplicationServices.h>
#import "MainView.h"
#import "Server.h"

@interface warpAppDelegate : NSObject<NSApplicationDelegate> {
	NSWindow *window;
	IBOutlet NSMenu *menu;
	Server* server;
}

@property (assign) IBOutlet NSWindow *window;

- (IBAction)connect:(id)sender;

@end
