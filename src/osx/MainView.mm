//
//  MainView.m
//  warp
//
//  Created by Nicholas Kostelnik on 11/09/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "MainView.h"


@implementation MainView

-(void)drawRect:(NSRect)rect
{
	NSImage *anImage = [NSImage imageNamed:@"bezel"];
	[[NSColor colorWithPatternImage:anImage] set];
	NSRectFill([self bounds]);
}

@end
