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
#import "Client.h"
#import "BlackHole.h"

@interface WarpAppDelegate : NSObject<NSApplicationDelegate> {
	NSWindow *window;
	IBOutlet NSWindow *connect_window;
	IBOutlet NSMenu *menu;
	IBOutlet NSMenuItem *recent_menu;
	IBOutlet NSTextField *address;
	IBOutlet MainView *input_view;
	
	BlackHole* black_hole;	
	Client *client;
	Server* server;
}

@property (assign) IBOutlet NSWindow *window;

- (IBAction)show_connect:(id)sender;
- (IBAction)connect:(id)sender;
- (IBAction)quit:(id)sender;

@end
