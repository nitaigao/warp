/*
 *  BlackHole.h
 *  warp
 *
 *  Created by Nicholas Kostelnik on 14/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>
#include "IClientCommand.h"

class BlackHole
{	
	
	typedef std::map<int, IClientCommand*> ClientCommandList;
	
public:
	
	BlackHole(Client* client, NSWindow* window) : client_(client), window_(window)
	{
		client_commands_[kCGEventKeyDown]						= new KeyDownClientCommand();
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
	
	void send_input() 
	{ 
		CGAssociateMouseAndMouseCursorPosition(false);
		CGDisplayHideCursor(kCGDirectMainDisplay);
	};
	
	void disable()
	{
		CGDisplayShowCursor(kCGDirectMainDisplay);
		CGAssociateMouseAndMouseCursorPosition(true);
		CGEventTapEnable(event_tap_, false);
		client_->disconnect();
		[window_ orderOut:nil];
	}
	
	void attach()
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
			exit(1);
		}
		
		CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, event_tap_, 0);
		CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
		CGEventTapEnable(event_tap_, true);
	}
	
private:
	
	Client* client_;
	CFMachPortRef event_tap_;
	NSWindow* window_;
	float last_click_;
	ClientCommandList client_commands_;
	
	static CGEventRef event_tap(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon)
	{
		BlackHole* black_hole = (BlackHole*)refcon;
		return black_hole->on_event(type, event);
	}
	
	CGEventRef on_event(CGEventType type, CGEventRef event)
	{		
		switch (type) {
				
			case kCGEventKeyDown:
			{
				CGEventFlags flags = CGEventGetFlags(event);
				CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
				
				if ((flags & kCGEventFlagMaskShift) && (flags & kCGEventFlagMaskCommand) && keycode == 14) // cmd-shift-e
				{
					disable();
				}
				
				std::clog << "key down" << keycode << std::endl;
				client_->send_key_down(flags, keycode);
				return NULL;
			}
		}
		
		if (client_commands_.find(type) != client_commands_.end())
		{
			return client_commands_[type]->Execute(event, client_);
		}
		
		return event;		
	}
};