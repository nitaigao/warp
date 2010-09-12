#import "MainApplication.h"

@interface NSApplication (NSPrivate)
- (BOOL)_handleKeyEquivalent:(NSEvent *)event;
- (void)_sendFinishLaunchingNotification;
@end

int MainApplicationMain(int argc, const char **argv)
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	NSDictionary *infoDictionary = [[NSBundle mainBundle] infoDictionary];
	Class principalClass =
	NSClassFromString([infoDictionary objectForKey:@"NSPrincipalClass"]);
	NSApplication *applicationObject = [principalClass sharedApplication];
	
	NSString *mainNibName = [infoDictionary objectForKey:@"NSMainNibFile"];
	NSNib *mainNib = [[NSNib alloc] initWithNibNamed:mainNibName bundle:[NSBundle mainBundle]];
	[mainNib instantiateNibWithOwner:applicationObject topLevelObjects:nil];
	
	if ([applicationObject respondsToSelector:@selector(run)])
	{
		[applicationObject
		 performSelectorOnMainThread:@selector(run)
		 withObject:nil
		 waitUntilDone:YES];
	}
	
	[mainNib release];
	[pool release];
	
	return 0;
}

@implementation MainApplication

- (void)run
{
	[[NSNotificationCenter defaultCenter] 
	 postNotificationName:NSApplicationWillFinishLaunchingNotification
	 object:NSApp];
	[[NSNotificationCenter defaultCenter]
	 postNotificationName:NSApplicationDidFinishLaunchingNotification
	 object:NSApp];
	
	shouldKeepRunning = YES;
	do
	{
		NSEvent *event = [self nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantFuture] inMode:NSDefaultRunLoopMode dequeue:YES];
		[self discardEventsMatchingMask:NSKeyUpMask beforeEvent:event];
		
		[self sendEvent:event];
		[self updateWindows];
	} while (shouldKeepRunning);
}


- (void)terminate:(id)sender
{
	shouldKeepRunning = NO;
}

@end

