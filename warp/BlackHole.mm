#include "BlackHole.h"

BlackHole::BlackHole(Client* client, NSWindow* window) : client_(client), window_(window), enabled_(false)
{
	client_commands_[kCGEventKeyDown]						= new KeyDownClientCommand(window);
	client_commands_[kCGEventKeyUp]							= new KeyUpClientCommand();
	client_commands_[kCGEventFlagsChanged]			= new FlagsChangedClientCommand();
	client_commands_[kCGEventLeftMouseUp]				= new LeftMouseUpClientCommand();
	client_commands_[kCGEventLeftMouseDown]			= new LeftMouseDownClientCommand();
	client_commands_[kCGEventLeftMouseDragged]	= new LeftMouseDraggedClientCommand();
	client_commands_[kCGEventRightMouseUp]			= new RightMouseUpClientCommand();
	client_commands_[kCGEventRightMouseDown]		= new RightMouseDownClientCommand();
	client_commands_[kCGEventRightMouseDragged]	= new RightMouseDraggedClientCommand();
	client_commands_[kCGEventMouseMoved]				= new MouseMovedClientCommand();
	client_commands_[kCGEventScrollWheel]				= new ScrollWheelClientCommand();
};

bool BlackHole::send_to(const std::string& host, unsigned int port)
{
	bool result = client_->connect_to(host, port);
	
	if (result)
	{
		enable();
	}
	
	return result;
}

void BlackHole::tap()
{		
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
	
	event_tap_ = CGEventTapCreate(kCGSessionEventTap,  kCGHeadInsertEventTap,  0, eventType, BlackHole::event_tap, this);
	
	if(!event_tap_)
	{
		std::cerr << "failed to create event tap" << std::endl;
	}
	
	CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, event_tap_, 0);
	CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
	CGEventTapEnable(event_tap_, true);
}

void BlackHole::disable()
{
	CGDisplayShowCursor(kCGDirectMainDisplay);
	CGAssociateMouseAndMouseCursorPosition(true);
	[window_ orderOut:nil];
	client_->disconnect();
	enabled_ = false;
}

CGEventRef BlackHole::event_tap(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon)
{
	BlackHole* black_hole = (BlackHole*)refcon;
	return black_hole->on_event(type, event);
}

void BlackHole::enable() 
{ 
	CGAssociateMouseAndMouseCursorPosition(false);
	CGDisplayHideCursor(kCGDirectMainDisplay);
	[window_ makeKeyAndOrderFront:nil];
	[[NSApplication sharedApplication] activateIgnoringOtherApps : YES];
	enabled_ = true;
};

CGEventRef BlackHole::on_event(CGEventType type, CGEventRef event)
{			
	scan_reconnect(type, event);
	return scan_input(type, event);
}

void BlackHole::scan_reconnect(CGEventType type, CGEventRef event)
{
	if (type == kCGEventKeyDown) 
	{
		CGEventFlags flags = CGEventGetFlags(event);
		CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
		
		if ((flags & kCGEventFlagMaskShift) && (flags & kCGEventFlagMaskCommand) && keycode == 14) // cmd-shift-e
		{
			if (enabled_ && client_->connected()) 
			{
				disable();
			}
			else if (!enabled_ && !client_->connected())
			{				
				if (client_->reconnect())
				{
					enable();
				}
			}
		}
	}
}

CGEventRef BlackHole::scan_input(CGEventType type, CGEventRef event)
{	
	if (type == kCGEventTapDisabledByTimeout)
	{
		tap();
	}

	if (enabled_ && client_commands_.find(type) != client_commands_.end())
	{
		client_commands_[type]->Execute(event, client_);
		event = NULL;
	}
	
	return event;		
}