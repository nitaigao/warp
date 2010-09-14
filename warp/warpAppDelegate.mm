//
//  warpAppDelegate.m
//  warp
//
//  Created by Nicholas Kostelnik on 11/09/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "warpAppDelegate.h"
#import <Carbon/Carbon.h>

@implementation warpAppDelegate

@synthesize window;

- (void)update {
	
	while (server->receive());
}

- (IBAction)show_connect:(id)sender {
	[connect_window makeKeyAndOrderFront:self]; 
}

- (IBAction)connect:(id)sender {
	[connect_window orderOut:self];
	
	if (client->attach([[address stringValue] cStringUsingEncoding:NSASCIIStringEncoding], 12345))
	{
		black_hole->send_input();
		//SetSystemUIMode(kUIModeContentHidden, kUIOptionDisableProcessSwitch	| kUIOptionDisableForceQuit);
		//[window orderFront:self];
	}
	else {
		[connect_window makeKeyAndOrderFront:self]; 
	}

}

- (IBAction)quit:(id)sender {
	[NSApp replyToApplicationShouldTerminate: YES]; 
	exit(0);
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {	
	server = new Server();
	server->start_listening(12345);
	
	client = new Client();
	[input_view set_client:client];
	
	black_hole = new BlackHole(client);
	black_hole->attach();
	
	[NSThread detachNewThreadSelector:@selector(update) toTarget:self withObject:nil];
	
	[window setAcceptsMouseMovedEvents:YES];	
	
	NSStatusItem* statusItem = [[[NSStatusBar systemStatusBar] statusItemWithLength:NSVariableStatusItemLength] retain];
	[statusItem setImage:[NSImage imageNamed:@"icon"]];
	[statusItem setHighlightMode:YES];
	[statusItem setEnabled:YES];
	[statusItem setMenu:menu];
	
	[NSEvent addGlobalMonitorForEventsMatchingMask:NSKeyDownMask handler:^(NSEvent*){
		
	}];
}



@end
