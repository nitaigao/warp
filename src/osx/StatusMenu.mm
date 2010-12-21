
#import "StatusMenu.h"

@implementation StatusMenu

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
		[recent_menu addItemWithTitle:item action:@selector(recent:) keyEquivalent:@""];
	}
}

- (void)awakeFromNib {
	[self init_main_menu];
	[self init_recent_list];
}

- (void)show_menu {
  [statusItem popUpStatusItemMenu:main_menu];
}

- (void)store_recent_list {
	NSMutableArray* recent_list = [[[NSMutableArray alloc] init] autorelease];
	
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

- (void)updateTheMenu:(NSString*)item_address
{  
  if ([network_seperator_item isHidden])
  {
    [network_seperator_item setHidden:FALSE];
  }
  
  NSMenuItem *old_item = [main_menu itemWithTitle:item_address];
	
	if (!old_item)
	{
    [main_menu insertItemWithTitle:item_address
                            action:@selector(recent:)
                     keyEquivalent:@"" 
                           atIndex:[main_menu indexOfItem:network_seperator_item] + 1];
    
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

- (IBAction)quit:(id)sender {
  [network stop];
  [NSApp performSelector:@selector(terminate:) withObject:nil afterDelay:0.0];
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
	
  [recent_menu insertItemWithTitle:item_address action:@selector(recent:) keyEquivalent:@"" atIndex:0];
	
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
