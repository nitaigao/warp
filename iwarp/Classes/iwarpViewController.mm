//
//  iwarpViewController.m
//  iwarp
//
//  Created by Nicholas Kostelnik on 27/09/2010.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "iwarpViewController.h"

#import <QuartzCore/QuartzCore.h>

#include "Constants.hpp"

@implementation iwarpViewController



/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}	
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

- (void)showView:(UIView*)newView {	
	[self.view addSubview:newView];
	
	// set up an animation for the transition between the views
	CATransition *animation = [CATransition animation];
	[animation setDuration:0.5];
	[animation setType:kCATransitionPush];
	[animation setSubtype:kCATransitionFromLeft];
	[animation setTimingFunction:[CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut]];
	
	[[self.view layer] addAnimation:animation forKey:@"SwitchToView1"];

}

- (IBAction)connect:(id)sender {
	[self showView:sessionView];
	[addressField resignFirstResponder];
	client->connect_to([[addressField text] UTF8String], SERVER_PORT);
}

bool moved = false;

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {	
	UITouch *touch = [touches anyObject];
	CGPoint currentPosition = [touch locationInView:sessionView];
	CGPoint previousPosition = [touch previousLocationInView: sessionView];
	
	float x = currentPosition.x - previousPosition.x;
	float y = currentPosition.y - previousPosition.y;
	
	client->send_mouse_moved(x, y);
	moved = true;
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	if(!moved) {
		UITouch *touch = [[event allTouches] anyObject];
		
		NSSet *allTouches = [event allTouches];
		
		if ([allTouches count] == 2) { // right click
      client->send_right_down();
			client->send_right_up();
		}
		else {
		  if (touch.tapCount == 2) {
  			client->send_left_double_click();
  		}
			
  		if (touch.tapCount == 1) {
  			client->send_left_down();
  			client->send_left_up();
  		}
		}
	}
	moved = false;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	[super viewDidLoad];
	client = new Client();
	[self.view addSubview:connectView];
	[addressField becomeFirstResponder];
}


// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation != UIInterfaceOrientationPortrait);
}

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}

@end
