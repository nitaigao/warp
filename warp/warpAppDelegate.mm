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

- (IBAction)connect:(id)sender {
	SetSystemUIMode(kUIModeContentHidden, kUIOptionDisableProcessSwitch	| kUIOptionDisableForceQuit);
	[window orderFront:self];
}

- (IBAction)quit:(id)sender {
	[NSApp replyToApplicationShouldTerminate: YES]; 
	exit(0);
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)app {
	return NSTerminateNow;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {	
	server = new Server();
	server->start_listening(12345);
	
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
