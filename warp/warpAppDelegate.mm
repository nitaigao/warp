//
//  warpAppDelegate.m
//  warp
//
//  Created by Nicholas Kostelnik on 11/09/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "warpAppDelegate.h"
#import <Carbon/Carbon.h>

#include "Constants.hpp"

@implementation WarpAppDelegate

- (void)server_update {
	while (!quit) {
    server->receive();
    client->update(1000);
	}
}

- (IBAction)show_connect:(id)sender {
	[[NSApplication sharedApplication] activateIgnoringOtherApps : YES];
	[connect_window makeKeyAndOrderFront:self];
}

- (void)recent:(id)sender {
	NSMenuItem* menu_item = sender;
	[status_menu add_recent_item:[menu_item title]];	
	black_hole->send_to([[menu_item title] cStringUsingEncoding:NSASCIIStringEncoding], SERVER_PORT);
}

- (IBAction)connect:(id)sender {
	[connect_window orderOut:self];
	
	if (black_hole->send_to([[address stringValue] cStringUsingEncoding:NSASCIIStringEncoding], SERVER_PORT)) {	
		[status_menu add_recent_item:[address stringValue]];
	}
	else {
		[connect_window makeKeyAndOrderFront:self]; 
	}
}

- (IBAction)quit:(id)sender {
  black_hole->disable();
	quit = true;
	exit(0);
}

- (void)screen_locked {
	black_hole->disable();
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {		
	server = new Server();
	server->start_listening(SERVER_PORT);
	
	client = new Client();
	[input_view set_client:client];
	
	black_hole = new BlackHole(client, connected_window);
	black_hole->tap();
	
	[NSThread detachNewThreadSelector:@selector(server_update) toTarget:self withObject:nil];
	
	NSDistributedNotificationCenter * center = [NSDistributedNotificationCenter defaultCenter];	
	[center addObserver:self selector:@selector(screen_locked) name:@"com.apple.screenIsLocked" object:nil];
}

@end
