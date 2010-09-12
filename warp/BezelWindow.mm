//
//  BezelWindow.m
//  warp
//
//  Created by Nicholas Kostelnik on 12/09/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "BezelWindow.h"


@implementation BezelWindow

- (id)initWithContentRect:(NSRect)contentRect styleMask:(NSUInteger)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag {
	self = [super
					initWithContentRect:contentRect
					styleMask:NSBorderlessWindowMask
					backing:bufferingType
					defer:flag];
	if (self) {
	[self setOpaque:NO];
	[self setBackgroundColor:[NSColor clearColor]];
	}
	return self;
}

- (BOOL)canBecomeKeyWindow {
	return YES;
}

- (void) lockMouse {
	CGRect mainMonitor = CGDisplayBounds(CGMainDisplayID());
  CGFloat monitorHeight = CGRectGetHeight(mainMonitor);
  CGFloat monitorWidth = CGRectGetWidth(mainMonitor);
  CGPoint center_screen;
  center_screen.x = monitorWidth / 2;
  center_screen.y = monitorHeight - (140 + self.frame.size.height / 2);
	
	CGWarpMouseCursorPosition(center_screen);
  CGAssociateMouseAndMouseCursorPosition(false);
  CGDisplayHideCursor(kCGDirectMainDisplay);
	
	[view allow_input:YES];
}

- (void)ensureFocus {	
	CGEventRef event = CGEventCreate(NULL);
  CGPoint point = CGEventGetLocation(event); 
  
  CGEventRef theEvent = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, point, kCGMouseButtonLeft);
  CGEventSetType(theEvent, kCGEventLeftMouseDown);
  CGEventPost(kCGHIDEventTap, theEvent);
  CFRelease(theEvent);
	[[NSApplication sharedApplication] activateIgnoringOtherApps : YES];
}

- (void)orderFront:(id)sender {
	[view allow_input:YES];
	[super orderFront:sender];
	[self setAcceptsMouseMovedEvents:YES];	
	[self lockMouse];
	[self ensureFocus];
}

@end
