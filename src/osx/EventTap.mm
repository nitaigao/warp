//
//  EventTap.m
//  warp
//
//  Created by Nicholas Kostelnik on 20/12/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "EventTap.h"
#import "TapEvent.h"

@implementation EventTap

@synthesize network;

- (id)init {
  self = [super init];
  [self enable_tap];
  return self;
}

- (void) enable_tap {
  CGEventType eventType = 
    (1 << kCGEventMouseMoved) | 
    (1 << kCGEventKeyDown) | 
    (1 << kCGEventKeyUp) |
    (1 << kCGEventFlagsChanged) | 
    (1 << kCGEventLeftMouseUp) | 
    (1 << kCGEventLeftMouseDown) | 
    (1 << kCGEventLeftMouseDragged) |
    (1 << kCGEventRightMouseUp) | 
    (1 << kCGEventRightMouseDown) |
    (1 << kCGEventRightMouseDragged) |
    (1 << kCGEventScrollWheel);
	
	event_tap_ = CGEventTapCreate(kCGSessionEventTap,  kCGHeadInsertEventTap,  0, eventType, on_tap_event, self);
	
	if(!event_tap_)
	{
		NSLog(@"failed to create event tap");
	}
	
	CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, event_tap_, 0);
	CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
	CGEventTapEnable(event_tap_, true);
}

CGEventRef on_tap_event(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon)
{
  EventTap* event_tap = (EventTap*)refcon;
  
  if (type == kCGEventTapDisabledByTimeout)
  {
    [event_tap enable_tap];
    return event;
  }
  
  if ([event_tap.network understands:type])
  {
    TapEvent* tap_event = [[TapEvent alloc] initWithType:type andEvent:event];
    [event_tap.network on_event:tap_event.type withEvent:tap_event.event];
    return NULL;
  }
  
	return event;
};

@end
