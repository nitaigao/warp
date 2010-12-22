//
//  EventTap.h
//  warp
//
//  Created by Nicholas Kostelnik on 20/12/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "Network.h"

@interface EventTap : NSObject {
  IBOutlet Network* network;
  CFMachPortRef event_tap_;
}

- (void)enable_tap;

CGEventRef on_tap_event(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon);

@end
