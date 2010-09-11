//
//  MainView.m
//  warp
//
//  Created by Nicholas Kostelnik on 11/09/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "MainView.h"


@implementation MainView

- (void)mouseUp:(NSEvent *)theEvent {
	client->send_left_up();
	NSLog(@"left up");
}

- (void)mouseDown:(NSEvent *)theEvent {
	client->send_left_down();
	NSLog(@"left down");
}

- (void)mouseDragged:(NSEvent *)theEvent {
	float x = [theEvent deltaX];
	float y = [theEvent deltaY];
	client->send_left_dragged(x, y);	
	NSLog(@"left dragged %f %f", x ,y);
}

- (void)rightMouseDown:(NSEvent *)theEvent {
	client->send_right_down();
	NSLog(@"right click");
}

- (void)rightMouseUp:(NSEvent *)theEvent {
	client->send_right_up();
	NSLog(@"right click");
}

- (void)rightMouseDragged:(NSEvent *)theEvent {
	float x = [theEvent deltaX];
	float y = [theEvent deltaY];
	client->send_right_dragged(x, y);	
	NSLog(@"right dragged %f %f", x ,y);
}

- (void)scrollWheel:(NSEvent *)theEvent {
	float x = [theEvent deltaX];
	float y = [theEvent deltaY];
	client->send_scroll_wheel(x, y);
	NSLog(@"scroll wheel");
}

- (void)mouseMoved:(NSEvent *)theEvent {
	float x = [theEvent deltaX];
	float y = [theEvent deltaY];
	client->send_mouse_moved(x, y);	
	NSLog(@"mouse moved %f %f", x ,y);
}

-(void)keyDown:(NSEvent*)theEvent {
	int keycode = [theEvent keyCode];
	client->send_key_down(keycode);
	NSLog(@"key down %d", keycode);
}

- (void)flagsChanged:(NSEvent *)theEvent {
	int keycode = [theEvent keyCode];
	unsigned int flags = [theEvent modifierFlags];
	client->send_flags(keycode, flags);
	NSLog(@"flags changed %d", [theEvent modifierFlags]);
}

- (BOOL)acceptsFirstResponder {
	client = new Client();
	client->attach();
	return YES;
}

@end
