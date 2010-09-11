//
//  MainView.m
//  iwarp
//
//  Created by Nicholas Kostelnik on 11/09/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "MainView.h"


@implementation MainView

- (void)checkinit {
	if (!client) {
		moved = false;
		client = new Client();
		client->attach();
	}
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event { 
	[self checkinit];
	NSLog(@"touch began");
}


- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {	
	[self checkinit];
	UITouch *touch = [touches anyObject];
	CGPoint currentPosition = [touch locationInView:self];
	CGPoint previousPosition = [touch previousLocationInView: self];
	
	float y = currentPosition.x - previousPosition.x;
	float x = currentPosition.y - previousPosition.y;

	client->send_mouse_moved(x, -y);
	moved = true;
	NSLog(@"touch moved");
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {	
	NSLog(@"touch cancelled");
}


- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	if(!moved) {
		[self checkinit];
		UITouch *touch = [[event allTouches] anyObject];
		
		if (touch.tapCount == 2) {
			client->send_left_double_click();
		}
		
		if (touch.tapCount == 1) {
			client->send_left_down();
			client->send_left_up();
		}
		NSLog(@"touch ended");
	}
	moved = false;
}

- (void)dealloc {
    [super dealloc];
}


@end
