
#import "StatusMenu.h"

@implementation StatusMenu

- (NSString*)recent_path {
	NSString* path = [[NSString alloc] initWithFormat:@"%@/Contents/Resources/recent.plist", [[NSBundle mainBundle] bundlePath]];
	if (![[[NSFileManager alloc ]init]fileExistsAtPath:path isDirectory:FALSE]) {
		[[[NSArray alloc] init]writeToFile:path atomically:YES];
	}
	return path;
}

- (void)init_recent_list {
	NSMutableArray* recent_list = [[NSMutableArray alloc] initWithContentsOfFile:[self recent_path]];
	
	if ([recent_list count] > 0)
	{
		[recent_menu removeItemAtIndex:0];
	}
	
	for(NSString* item in recent_list) {
		[recent_menu addItemWithTitle:item action:@selector(recent:) keyEquivalent:@""];
	}
}

- (IBAction)menuClick:(id)sender {
  [statusItem popUpStatusItemMenu:main_menu];
}

- (void)init_main_menu {
	statusItem = [[[NSStatusBar systemStatusBar] statusItemWithLength:NSVariableStatusItemLength] retain];
  [statusItem setImage:[NSImage imageNamed:@"menu"]];
  [statusItem setHighlightMode:YES];
	[statusItem setEnabled:YES];
  [statusItem setAction:@selector(menuClick:)];
  [statusItem setTarget:self];
}

- (void)awakeFromNib {
	[self init_main_menu];
	[self init_recent_list];
}

- (void)store_recent_list {
	NSMutableArray* recent_list = [[NSMutableArray alloc] init];
	
	for (NSMenuItem* menu_item in [recent_menu itemArray]) {
		[recent_list addObject:[menu_item title]];
	}
	
	[recent_list writeToFile:[self recent_path] atomically:YES];
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
	
	[self store_recent_list];
}

@end
