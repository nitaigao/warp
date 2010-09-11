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
  //CGDisplayHideCursor(kCGDirectMainDisplay);
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	[window setAcceptsMouseMovedEvents:YES];
	//[window setStyleMask:NSBorderlessWindowMask];
	
	[self lockMouse];
	[[NSApplication sharedApplication] activateIgnoringOtherApps : YES];
}
@end
