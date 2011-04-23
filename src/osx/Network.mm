#import "Network.h"
#import "Exit.h"
#import "ZeroMQContext.hpp"

@implementation Network

- (id) init {
  self = [super init];
  quit = false;
  
  ZeroMQContext::init();
  entrance = new Entrance();
    
  [NSThread detachNewThreadSelector:@selector(exit_thread) toTarget:self withObject:nil];
  [NSThread detachNewThreadSelector:@selector(broadcast_thread) toTarget:self withObject:nil];
  [NSThread detachNewThreadSelector:@selector(subscriber_thread) toTarget:self withObject:nil];
  [NSThread detachNewThreadSelector:@selector(update_thread) toTarget:self withObject:nil];

  return self;
}

- (void)quit {
  quit = true;
  sleep(1);
  [NSApp performSelector:@selector(terminate:) withObject:nil afterDelay:0.0]; 
}

- (void)recent:(NSString*)address {
  [self connect_to:address withPort:SERVER_PORT]; 
}

- (void)awakeFromNib {
  [status_menu set_delegate:self]; 
}

- (void)on_event:(CGEventType)eventType withEvent:(CGEventRef)event {
  entrance->on_event(eventType, event);
}

- (void)connected_test {
  if (![self is_connected]) {
    [[NSApplication sharedApplication] activateIgnoringOtherApps : YES];
    CGAssociateMouseAndMouseCursorPosition(false);
    CGDisplayHideCursor(kCGDirectMainDisplay);
    [bezel_window show_bezel:true];
  }
  else {
    [bezel_window show_bezel:false];
    CGDisplayShowCursor(kCGDirectMainDisplay);
    CGAssociateMouseAndMouseCursorPosition(true);
  }  
}

- (void)connect_to:(NSString*)address withPort:(unsigned int)port {
  [self connected_test];
  [status_menu add_recent_item:address];
  entrance->connect_to([address cStringUsingEncoding:NSASCIIStringEncoding], port);
}

- (void)toggle {
  [self connected_test];
  entrance->toggle();
}

- (bool)is_connected {
  return entrance->is_enabled();
}

- (bool)understands:(CGEventType)eventType {
  return entrance->understands(eventType);
}

- (void)add_network_item:(NSString*)address {
  [status_menu add_network_item:address time:4000];
}

- (void)subscriber_thread {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	while (!quit) {
    std::string host = multicast.receive();
    if (host.length() > 0)
    {
      [self performSelectorOnMainThread:@selector(add_network_item:) withObject:[NSString stringWithUTF8String:host.c_str()] waitUntilDone:false];
    }
    sleep(1);
	}
  [pool release];  
}

- (void)broadcast_thread {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	while (!quit) {
    multicast.broadcast();
    sleep(1);
	}
  [pool release];  
}

- (void)exit_thread {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  Exit exit;
	while (!quit) {
    exit.receive_input();
	}
  exit.shutdown();
  [pool release];
}

- (void)update {
  [status_menu update:1000];
}

- (void)update_thread {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	while (!quit) {
    [self performSelectorOnMainThread:@selector(update) withObject:nil waitUntilDone:false];
    sleep(1);
	}
  [pool release];
};

@end
