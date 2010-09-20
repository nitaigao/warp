/*
 *  IClientCommand.h
 *  warp
 *
 *  Created by Nicholas Kostelnik on 16/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

#include "Time.h"

class IClientCommand
{
	
public:
	
	virtual CGEventRef Execute(CGEventRef event, Client* client) = 0;
		
};

class KeyDownClientCommand : public IClientCommand
{
	
public:
	
	KeyDownClientCommand(NSWindow* window) : window_(window) { };
	
	CGEventRef Execute(CGEventRef event, Client* client)
	{
		CGEventFlags flags = CGEventGetFlags(event);
		CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
		
		std::clog << "key down" << keycode << std::endl;
		client->send_key_down(flags, keycode);
		return NULL;
	}
	
private:
	
	NSWindow* window_;
};

class KeyUpClientCommand : public IClientCommand
{
	
public:
	
	CGEventRef Execute(CGEventRef event, Client* client)
	{
		std::clog << "key up" << std::endl;
		CGEventFlags flags = CGEventGetFlags(event);
		CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
		client->send_key_up(flags, keycode);
		return NULL;
	}
};

class FlagsChangedClientCommand : public IClientCommand
{
	
public:
	
	CGEventRef Execute(CGEventRef event, Client* client)
	{
		std::clog << "flags changed" << std::endl;
		CGEventFlags flags = CGEventGetFlags(event);
		CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
		client->send_flags(keycode, flags);
		return NULL;
	}
};

class LeftMouseUpClientCommand : public IClientCommand
{
	
public:
	
	CGEventRef Execute(CGEventRef event, Client* client)
	{
		std::clog << "left mouse up" << std::endl;
		client->send_left_up();
		return NULL;
	}
};

class LeftMouseDownClientCommand : public IClientCommand
{
	
public:
	
	LeftMouseDownClientCommand()
	{
		last_click_ = timeGetTime();
	}
	
	CGEventRef Execute(CGEventRef event, Client* client)
	{
		std::clog << "left mouse down" << std::endl;
	
		unsigned int time_now = timeGetTime();
		unsigned int delta = time_now - last_click_;
		
		std::clog << delta << std::endl;
				
		if (delta < 300)
		{
			std::clog << "detected double click" << std::endl;
			client->send_left_double_click();
		}
		else 
		{
			client->send_left_down();
		}
		
		last_click_ = time_now;

		return NULL;
	}
	
private:
	
	unsigned int last_click_;
};

class LeftMouseDraggedClientCommand : public IClientCommand
{
	
public:
	
	CGEventRef Execute(CGEventRef event, Client* client)
	{
		std::clog << "left mouse dragged" << std::endl;
		int x = CGEventGetIntegerValueField(event, kCGMouseEventDeltaX);
		int y = CGEventGetIntegerValueField(event, kCGMouseEventDeltaY);
		client->send_left_dragged(x, y);
		return NULL;
	}
};

class RightMouseUpClientCommand : public IClientCommand
{
	
public:
	
	CGEventRef Execute(CGEventRef event, Client* client)
	{
		std::clog << "right mouse up" << std::endl;
		client->send_right_up();
		return NULL;
	}
};

class RightMouseDownClientCommand : public IClientCommand
{
	
public:
	
	CGEventRef Execute(CGEventRef event, Client* client)
	{
		std::clog << "right mouse down" << std::endl;
		client->send_right_down();
		return NULL;
	}
};

class RightMouseDraggedClientCommand : public IClientCommand
{
	
public:
	
	CGEventRef Execute(CGEventRef event, Client* client)
	{
		std::clog << "right mouse dragged" << std::endl;
		int x = CGEventGetIntegerValueField(event, kCGMouseEventDeltaX);
		int y = CGEventGetIntegerValueField(event, kCGMouseEventDeltaY);
		client->send_right_dragged(x, y);
		return NULL;
	}
};

class MouseMovedClientCommand : public IClientCommand
{
	
public:
	
	CGEventRef Execute(CGEventRef event, Client* client)
	{
		std::clog << "mouse moved" << std::endl;				
		int x = CGEventGetIntegerValueField(event, kCGMouseEventDeltaX);
		int y = CGEventGetIntegerValueField(event, kCGMouseEventDeltaY);
		client->send_mouse_moved(x, y);
		std::clog << x << std::endl;
		return NULL;
		
	}
};

class ScrollWheelClientCommand : public IClientCommand
{
	
public:
	
	CGEventRef Execute(CGEventRef event, Client* client)
	{
		std::clog << "scroll wheel" << std::endl;
		int x = CGEventGetIntegerValueField(event, kCGScrollWheelEventDeltaAxis2);
		int y = CGEventGetIntegerValueField(event, kCGScrollWheelEventDeltaAxis1);
		client->send_scroll_wheel(x, y);
		return NULL;
	}
};