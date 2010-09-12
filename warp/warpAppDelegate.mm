//
//  warpAppDelegate.m
//  warp
//
//  Created by Nicholas Kostelnik on 11/09/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "warpAppDelegate.h"

@implementation warpAppDelegate

@synthesize window;

- (void) lockMouse {
	CGRect mainMonitor = CGDisplayBounds(CGMainDisplayID());
  CGFloat monitorHeight = CGRectGetHeight(mainMonitor);
  CGFloat monitorWidth = CGRectGetWidth(mainMonitor);
  CGPoint center_screen;
  center_screen.x = monitorWidth / 2;
  center_screen.y = monitorHeight / 2;
	
	CGWarpMouseCursorPosition(center_screen);
  CGAssociateMouseAndMouseCursorPosition(false);
  CGDisplayHideCursor(kCGDirectMainDisplay);
}

- (void)ensureFocus {
	[[NSApplication sharedApplication] activateIgnoringOtherApps : YES];
	
	CGEventRef event = CGEventCreate(NULL);
  CGPoint point = CGEventGetLocation(event); 
  
  CGEventRef theEvent = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, point, kCGMouseButtonLeft);
  CGEventSetType(theEvent, kCGEventLeftMouseDown);
  CGEventPost(kCGHIDEventTap, theEvent);
  CFRelease(theEvent);
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	[window setAcceptsMouseMovedEvents:YES];
	
	NSMenu		*menu;
	NSStatusItem* statusItem = [[[NSStatusBar systemStatusBar] statusItemWithLength:NSVariableStatusItemLength] retain];
	
	[statusItem setImage:[NSImage imageNamed:@"test"]];
	[statusItem setHighlightMode:YES];
	
	menu = [[NSMenu alloc] initWithTitle:@""];
	[menu addItemWithTitle:@"Hi" action:NULL keyEquivalent:@""];
	[statusItem setMenu:menu];
	[menu release];
	
	[window setStyleMask:NSBorderlessWindowMask];
	
	[self lockMouse];
	[self ensureFocus];
}
@end
