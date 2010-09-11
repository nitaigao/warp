//
//  warpAppDelegate.h
//  warp
//
//  Created by Nicholas Kostelnik on 11/09/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include <ApplicationServices/ApplicationServices.h>

@interface warpAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
}

@property (assign) IBOutlet NSWindow *window;

@end
