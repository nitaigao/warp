//
//  warpAppDelegate.h
//  warp
//
//  Created by Nicholas Kostelnik on 11/09/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "Network.h"
#import "EventTap.h"

#import "StatusMenu.h"

@interface WarpAppDelegate : NSObject<NSApplicationDelegate> {
  
  IBOutlet Network *network;
  IBOutlet EventTap *event_tap;
  IBOutlet StatusMenu *status_menu;
}

@end
