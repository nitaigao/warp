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
#import "StatusMenu.h"


#import "Server.h"
#import "Client.h"
#import "BlackHole.h"

@interface WarpAppDelegate : NSObject<NSApplicationDelegate> {
	IBOutlet NSWindow *connected_window;
	IBOutlet NSWindow *connect_window;
	
	IBOutlet NSTextField *address;

	IBOutlet MainView *input_view;
	IBOutlet StatusMenu* status_menu; 
		
	BlackHole* black_hole;	
	Client *client;
	Server* server;
}

- (IBAction)show_connect:(id)sender;
- (IBAction)connect:(id)sender;
- (IBAction)quit:(id)sender;

@end
