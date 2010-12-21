#ifndef ICLIENTCOMMAND_HPP
#define ICLIENTCOMMAND_HPP

	#include <ApplicationServices/ApplicationServices.h>
	#include <Carbon/Carbon.h>

	#include "Time.h"
	#include "Constants.hpp"
	#include "Client.h"

	#include "KeyCodes.hpp"

	class IClientCommand
	{
		
	public:
		
		virtual bool Execute(CGEventRef event, Client* client) = 0;
			
	};

	class KeyDownClientCommand : public IClientCommand
	{
		
	public:
		
		KeyDownClientCommand() { };
		
		bool Execute(CGEventRef event, Client* client)
		{
      
			CGEventFlags flags = CGEventGetFlags(event);
			CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
			return client->send_key_down(flags, KeyCodes().osx_to_generic(keycode));
		}
  
	};

	class KeyUpClientCommand : public IClientCommand
	{
		
	public:
		
		bool Execute(CGEventRef event, Client* client)
		{
			CGEventFlags flags = CGEventGetFlags(event);
			CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);			
			return client->send_key_up(flags, KeyCodes().osx_to_generic(keycode));
		}
	};

	class FlagsChangedClientCommand : public IClientCommand
	{
		
	public:
		
		bool Execute(CGEventRef event, Client* client)
		{
			CGEventFlags flags = CGEventGetFlags(event);
			CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
			return client->send_flags(keycode, flags);
		}
	};

	class LeftMouseUpClientCommand : public IClientCommand
	{
		
	public:
		
		bool Execute(CGEventRef event, Client* client)
		{
			return client->send_left_up();
		}
	};

	class LeftMouseDownClientCommand : public IClientCommand
	{
		
	public:
		
		LeftMouseDownClientCommand()
		{
			last_click_ = Time::get();
		}
		
		bool Execute(CGEventRef event, Client* client)
		{	
			unsigned int time_now = Time::get();
			unsigned int delta = time_now - last_click_;
					
			last_click_ = time_now;
			
			if (delta < DOUBLE_CLICK_THRESHOLD)
			{
				return client->send_left_double_click();
			}
			else 
			{
				return client->send_left_down();
			}
		}
		
	private:
		
		unsigned int last_click_;
	};

	class LeftMouseDraggedClientCommand : public IClientCommand
	{
		
	public:
		
		bool Execute(CGEventRef event, Client* client)
		{
			int x = CGEventGetIntegerValueField(event, kCGMouseEventDeltaX);
			int y = CGEventGetIntegerValueField(event, kCGMouseEventDeltaY);
			return client->send_left_dragged(x, y);
		}
	};

	class RightMouseUpClientCommand : public IClientCommand
	{
		
	public:
		
		bool Execute(CGEventRef event, Client* client)
		{
			return client->send_right_up();
		}
	};

	class RightMouseDownClientCommand : public IClientCommand
	{
		
	public:
		
		bool Execute(CGEventRef event, Client* client)
		{
			return client->send_right_down();
		}
	};

	class RightMouseDraggedClientCommand : public IClientCommand
	{
		
	public:
		
		bool Execute(CGEventRef event, Client* client)
		{
			int x = CGEventGetIntegerValueField(event, kCGMouseEventDeltaX);
			int y = CGEventGetIntegerValueField(event, kCGMouseEventDeltaY);
			return client->send_right_dragged(x, y);
		}
	};

	class MouseMovedClientCommand : public IClientCommand
	{
		
	public:
		
		bool Execute(CGEventRef event, Client* client)
		{			
      int x = CGEventGetIntegerValueField(event, kCGMouseEventDeltaX);
			int y = CGEventGetIntegerValueField(event, kCGMouseEventDeltaY);
			return client->send_mouse_moved(x, y);
		}
	};

	class ScrollWheelClientCommand : public IClientCommand
	{
		
	public:
		
		bool Execute(CGEventRef event, Client* client)
		{
			int x = CGEventGetIntegerValueField(event, kCGScrollWheelEventDeltaAxis2);
			int y = CGEventGetIntegerValueField(event, kCGScrollWheelEventDeltaAxis1);
			return client->send_scroll_wheel(x, y);
		}
	};

#endif