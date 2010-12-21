#import "EventTap.h"

@implementation EventTap

- (id)init {
  self = [super init];
  [self enable_tap];
  return self;
}

- (void)enable_tap {
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

- (bool)toggle:(CGEventType)type withEvent:(CGEventRef)event {
  if (type == kCGEventKeyDown) 
  {
    CGEventFlags flags = CGEventGetFlags(event);
    CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    
    if ((flags & kCGEventFlagMaskShift) && (flags & kCGEventFlagMaskCommand) && keycode == 14) // cmd-shift-e
    {
      [network toggle];
      return true;
    }
  }
  return false;
}

- (CGEventRef)on_event:(CGEventType)type withEvent:(CGEventRef)event {
  if ([self toggle:type withEvent:event]) {
    return NULL; 
  }

  if ([network understands:type])
  {
    [network on_event:type withEvent:event];
    return NULL;
  }
  
	return event;  
}

CGEventRef on_tap_event(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon)
{
  EventTap* event_tap = (EventTap*)refcon;
  
  if (type == kCGEventTapDisabledByTimeout)
  {
    [event_tap enable_tap];
    return event;
  }
  
  return [event_tap on_event:type withEvent: event];
};


@end
