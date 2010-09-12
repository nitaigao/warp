//
//  MainApplication.h
//  warp
//
//  Created by Nicholas Kostelnik on 12/09/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

int MainApplicationMain(int argc, const char **argv);

@interface MainApplication : NSApplication
{
	bool shouldKeepRunning;
}

- (void)run;
- (void)terminate:(id)sender;

@end


