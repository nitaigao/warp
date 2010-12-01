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

#import "Exit.h"
#import "Client.h"
#import "Entrance.h"

@interface WarpAppDelegate : NSObject<NSApplicationDelegate> {
	IBOutlet NSWindow *connected_window;
	IBOutlet NSWindow *connect_window;
	
	IBOutlet NSTextField *address;

	IBOutlet MainView *input_view;
	IBOutlet StatusMenu* status_menu; 
		
	Entrance* entrance;	
	Exit* exit_;
	
	Client *client;
	
	bool quit;
	int search_count;
}

- (IBAction)show_connect:(id)sender;
- (IBAction)connect:(id)sender;
- (IBAction)quit:(id)sender;

@end
