
#import "StatusMenu.h"

@implementation StatusMenu

- (NSString*)recent_path {
	NSString* path = [[[NSString alloc] initWithFormat:@"%@/Contents/Resources/recent.plist", [[NSBundle mainBundle] bundlePath]] autorelease];
	if (![[[[NSFileManager alloc] init] autorelease] fileExistsAtPath:path isDirectory:FALSE]) {
		[[[NSArray alloc] init]writeToFile:path atomically:YES];
	}
	return path;
}

- (void)init_recent_list {
	NSMutableArray* recent_list = [[[NSMutableArray alloc] initWithContentsOfFile:[self recent_path]] autorelease];
	
	if ([recent_list count] > 0)
	{
		[recent_menu removeItemAtIndex:0];
	}
	
	for(NSString* item in recent_list) {
		[recent_menu addItemWithTitle:item action:@selector(recent:) keyEquivalent:@""];
	}
}

- (void)show_menu {
  is_open = true;
  [statusItem popUpStatusItemMenu:main_menu];
  is_open = false;
}

- (void)init_main_menu {
	statusItem = [[[NSStatusBar systemStatusBar] statusItemWithLength:NSVariableStatusItemLength] retain];
  [statusItem setImage:[NSImage imageNamed:@"menu"]];
  [statusItem setHighlightMode:YES];
	[statusItem setEnabled:YES];
  [statusItem setAction:@selector(refresh:)];
}

- (bool)isOpen {
  return is_open;
}

- (void)awakeFromNib {
	[self init_main_menu];
	[self init_recent_list];
}

- (void)store_recent_list {
	NSMutableArray* recent_list = [[[NSMutableArray alloc] init] autorelease];
	
	for (NSMenuItem* menu_item in [recent_menu itemArray]) {
		[recent_list addObject:[menu_item title]];
	}
	
	[recent_list writeToFile:[self recent_path] atomically:YES];
}

- (void)start_searching {
  [[main_menu itemWithTitle:@"Exit: Active"] setTitle:@"Searching..."];
}

- (void)stop_searching {
  [[main_menu itemWithTitle:@"Searching..."] setTitle:@"Exit: Active"];
}


- (void)updateTheMenu:(NSString*)item_address
{  
  NSMenuItem *old_item = [main_menu itemWithTitle:item_address];
	
	if (!old_item)
	{
    [main_menu insertItemWithTitle:item_address
                            action:@selector(recent:)
                     keyEquivalent:@"" 
                           atIndex:[main_menu indexOfItemWithTitle:@"Network"] + 1];
    
    [main_menu update];
	}
}

- (void)add_network_item:(NSString*)item_address {
  [[NSRunLoop currentRunLoop] performSelector:@selector(updateTheMenu:) 
                                       target:self 
                                     argument:item_address 
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
	
	[recent_menu addItemWithTitle:item_address action:@selector(recent:) keyEquivalent:@""];
	
	if ([recent_menu numberOfItems] > 5)
	{
		[recent_menu removeItemAtIndex:0];
	}
}

@end
