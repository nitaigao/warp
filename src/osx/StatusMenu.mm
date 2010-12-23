
#import "StatusMenu.h"
#import "Pair.h"

@implementation StatusMenu

- (void)set_delegate:(id)theDelegate {
  delegate = theDelegate; 
}

- (IBAction)quit:(id)sender {
  [delegate performSelector:@selector(quit)];
}

- (IBAction)recent:(id)sender {
  NSMenuItem* menu_item = sender;
	[self add_recent_item:[menu_item title]];	
  [delegate performSelector:@selector(recent:) withObject:[menu_item title]];
}

- (NSString*)recent_path {
	NSString* path = [[[NSString alloc] initWithFormat:@"%@/Contents/Resources/recent.plist", [[NSBundle mainBundle] bundlePath]] autorelease];
	if (![[[[NSFileManager alloc] init] autorelease] fileExistsAtPath:path isDirectory:FALSE]) {
		[[[NSArray alloc] init]writeToFile:path atomically:YES];
	}
	return path;
}

- (void)init_main_menu {
	statusItem = [[[NSStatusBar systemStatusBar] statusItemWithLength:NSVariableStatusItemLength] retain];
  [statusItem setImage:[NSImage imageNamed:@"menu"]];
  [statusItem setHighlightMode:YES];
	[statusItem setEnabled:YES];
  [statusItem setTarget:self];
  [statusItem setAction:@selector(show_menu)];
}

- (void)init_recent_list {
	NSMutableArray* recent_list = [[[NSMutableArray alloc] initWithContentsOfFile:[self recent_path]] autorelease];
	
	if ([recent_list count] > 0)
	{
		[recent_menu removeItemAtIndex:0];
	}
	
	for(NSString* item in recent_list) { 
    [[recent_menu addItemWithTitle:item action:@selector(recent:) keyEquivalent:@""] setTarget:self];
	}
}

- (id)init {
  self = [super init];
  network_items = [[NSMutableArray alloc] init];
  return self;
}

- (void)awakeFromNib {
	[self init_main_menu];
	[self init_recent_list];
}

- (void)show_menu {
  [statusItem popUpStatusItemMenu:main_menu];
}

- (void)update:(int)delta_milliseconds {
  for (Pair* pair in network_items) {
    pair.time = pair.time - delta_milliseconds;    
    if (pair.time <= 0)
    {
      [self remove_network_item:pair.value];
      [network_items removeObject:pair];
    }
  }
}

- (void)store_recent_list {
	NSMutableArray* recent_list = [[NSMutableArray alloc] init];
	
	for (NSMenuItem* menu_item in [recent_menu itemArray]) {
		[recent_list addObject:[menu_item title]];
	}
	
	[recent_list writeToFile:[self recent_path] atomically:YES];
}

- (void)start_searching {
  [network_item setTitle:@"Warp: Searching..."];
}

- (void)stop_searching {
  [network_item setTitle:@"Warp: Active"];
}

- (void)updateTheMenu:(Pair*)pair {
  if ([network_seperator_item isHidden])
  {
    [network_seperator_item setHidden:FALSE];
  }
  
  for(Pair* old_pair in network_items)
  {
    if ([old_pair.value isEqualToString:pair.value])
    {
      old_pair.time = pair.time; 
    }
  }
  
  NSMenuItem *old_item = [main_menu itemWithTitle:pair.value];
	
	if (!old_item)
	{
    [[main_menu insertItemWithTitle:pair.value
                            action:@selector(recent:)
                     keyEquivalent:@"" 
                            atIndex:[main_menu indexOfItem:network_seperator_item] + 1]setTarget:self];
    
    [network_items addObject:pair];
    [main_menu update];
	}
}

- (void)remove_item:(NSString*)item_address {
  NSMenuItem* item = [main_menu itemWithTitle:item_address];
  [main_menu removeItem:item];
  
  if (![network_items count]) {
    [network_seperator_item setHidden:true];
  }
}

- (void)remove_network_item:(NSString*)item_address {
  [[NSRunLoop currentRunLoop] performSelector:@selector(remove_item:) 
                                       target:self 
                                     argument:item_address 
                                        order:0 
                                        modes:[NSArray arrayWithObject:NSEventTrackingRunLoopMode]];
}

- (void)add_network_item:(NSString*)item_address time:(int)time {
  Pair *pair = [[Pair alloc] initWithTime:time andValue:item_address];
  
  [[NSRunLoop currentRunLoop] performSelector:@selector(updateTheMenu:) 
                                       target:self 
                                     argument:pair 
                                        order:0 
                                        modes:[NSArray arrayWithObject:NSEventTrackingRunLoopMode]];
}

- (void)add_recent_item:(NSString*)item_address {
	NSMenuItem *empty_item = [recent_menu itemWithTitle:@"Empty"];
	
	if (empty_item)
	{
		[recent_menu removeItem:empty_item];
	}
	
	NSMenuItem *old_item = [recent_menu itemWithTitle:item_address];
	
	if (old_item)
	{
		[recent_menu removeItem:old_item];
	}	
	
  [[recent_menu insertItemWithTitle:item_address action:@selector(recent:) keyEquivalent:@"" atIndex:0] setTarget:self];
	
	if ([recent_menu numberOfItems] > 5)
	{
		[recent_menu removeItemAtIndex:0];
	}
  
  NSMutableArray* recent_list = [[[NSMutableArray alloc] init] autorelease];
  for (NSMenuItem *menu_item in [recent_menu itemArray])
  {
    [recent_list addObject:[menu_item title]];
  }
  [recent_list writeToFile:[self recent_path] atomically:YES];
}

@end
