#ifndef OSX_EXIT_COMMANDS_HPP
#define OSX_EXIT_COMMANDS_HPP

#include "IExitCommand.hpp"
#include <ApplicationServices/ApplicationServices.h>
#include <iostream>
#include "KeyCodes.hpp"

	void PostMouseEvent(CGMouseButton button, CGEventType type, const CGPoint point, int click_count = 1) 
	{
		static int eventNumber = 0;
		CGEventRef theEvent = CGEventCreateMouseEvent(NULL, type, point, button);
		CGEventSetIntegerValueField(theEvent, kCGMouseEventClickState, click_count) ;
		CGEventSetIntegerValueField(theEvent, kCGMouseEventNumber, ++eventNumber); 
		CGEventSetType(theEvent, type);
		CGEventPost(kCGHIDEventTap, theEvent);
		CFRelease(theEvent);
	}

	class LeftUpCommand : public IExitCommand
	{
		
	public:
		
		static int type() { return LEFT_UP; };
		
		void Execute(const Message& message) 
		{ 
			CGEventRef event = CGEventCreate(NULL);
			CGPoint point = CGEventGetLocation(event); 
			PostMouseEvent(kCGMouseButtonLeft, kCGEventLeftMouseUp, point);
		};
		
	};

	class LeftDownCommand : public IExitCommand
	{
		
	public:
		
		static int type() { return LEFT_DOWN; };
		
		void Execute(const Message& message) 
		{ 
			CGEventRef event = CGEventCreate(NULL);
			CGPoint point = CGEventGetLocation(event); 
			PostMouseEvent(kCGMouseButtonLeft, kCGEventLeftMouseDown, point);
		};
		
	};


	class RightUpCommand : public IExitCommand
	{
		
	public:
		
		static int type() { return RIGHT_UP; };
		
		void Execute(const Message& message) 
		{ 
			CGEventRef event = CGEventCreate(NULL);
			CGPoint point = CGEventGetLocation(event); 
			PostMouseEvent(kCGMouseButtonRight, kCGEventRightMouseUp, point);
		};
		
	};

	class RightDownCommand : public IExitCommand
	{
		
	public:
		
		static int type() { return RIGHT_DOWN; };
		
		void Execute(const Message& message)
		{
			CGEventRef event = CGEventCreate(NULL);
			CGPoint point = CGEventGetLocation(event); 
			PostMouseEvent(kCGMouseButtonRight, kCGEventRightMouseDown, point);
		};
	};

	class KeyUpCommand : public IExitCommand
	{
		
	public:
		
		static int type() { return KEY_UP; };
		
		void Execute(const Message& message)
		{
			CGEventRef e = CGEventCreateKeyboardEvent (NULL, KeyCodes().generic_to_osx(message.key_code), false);
			CGEventSetFlags(e, (CGEventFlags)message.flags);
			CGEventPost(kCGSessionEventTap, e);
			CFRelease(e);
			
		};
		
	};

	class KeyDownCommand : public IExitCommand
	{
		
	public:
		
		static int type() { return KEY_DOWN; };
		
		void Execute(const Message& message)
		{
			CGEventRef e = CGEventCreateKeyboardEvent (NULL, KeyCodes().generic_to_osx(message.key_code), true);
			CGEventSetFlags(e, (CGEventFlags)message.flags);
			CGEventPost(kCGSessionEventTap, e);
			CFRelease(e);
		};
		
	};

	class MouseMovedCommand : public IExitCommand
	{
		
	public:
		
		static int type() { return MOUSE_MOVE; };
		
		void Execute(const Message& message)
		{
			CGEventRef ourEvent = CGEventCreate(NULL);
			CGPoint epoint = CGEventGetLocation(ourEvent);
			
			CGDisplayModeRef mode = CGDisplayCopyDisplayMode(kCGDirectMainDisplay);
			int width = CGDisplayModeGetWidth(mode);
			int height = CGDisplayModeGetHeight(mode);
			
			CGPoint point;
			point.x = epoint.x + message.x;
			point.y = epoint.y + message.y;
			
			point.x = (point.x < 0) ? 0 : (point.x > width) ? width - 1 : point.x;
			point.y = (point.y < 0) ? 0 : (point.y > height) ? height - 2 : point.y;
			PostMouseEvent(kCGMouseButtonCenter, kCGEventMouseMoved, point);
		};
		
	};

	class LeftDraggedCommand : public IExitCommand
	{
		
	public:
		
		static int type() { return LEFT_DRAGGED; };
		
		void Execute(const Message& message) 
		{
			CGEventRef ourEvent = CGEventCreate(NULL);
			CGPoint epoint = CGEventGetLocation(ourEvent);
			
			CGPoint point;
			point.x = epoint.x + message.x;
			point.y = epoint.y + message.y;
			PostMouseEvent(kCGMouseButtonLeft, kCGEventLeftMouseDragged, point);
		};
		
	};

	class RightDraggedCommand : public IExitCommand
	{
		
	public:
		
		static int type() { return RIGHT_DRAGGED; };
		
		void Execute(const Message& message)
		{
			CGEventRef ourEvent = CGEventCreate(NULL);
			CGPoint epoint = CGEventGetLocation(ourEvent);
			
			CGPoint point;
			point.x = epoint.x + message.x;
			point.y = epoint.y + message.y;
			PostMouseEvent(kCGMouseButtonRight, kCGEventRightMouseDragged, point);
		};
		
	};

	class FlagsChangedCommand : public IExitCommand
	{
		
	public:
		
		static int type() { return FLAGS_CHANGED; };
		
		void Execute(const Message& message)
		{
			CGEventRef e = CGEventCreateKeyboardEvent (NULL, message.key_code, true);
			CGEventSetFlags(e, (CGEventFlags)message.flags);
			CGEventPost(kCGSessionEventTap, e);
			CFRelease(e);
		};
		
	};

	class ScrollWheelCommand : public IExitCommand
	{
		
	public:
		
		static int type() { return SCROLL_WHEEL; };
		
		void Execute(const Message& message)
		{
			CGEventRef ourEvent = CGEventCreate(NULL);
			CGPoint point = CGEventGetLocation(ourEvent);
			
			CGEventRef e = CGEventCreateScrollWheelEvent(NULL, kCGScrollEventUnitPixel, 1, 2);
			
			CGEventSetType(e, kCGEventScrollWheel);
			CGEventSetIntegerValueField(e, kCGScrollWheelEventDeltaAxis1, message.y);
			CGEventPost(kCGSessionEventTap, e);
			CFRelease(e);
		};
		
	};

	class LeftDoubleClickCommand : public IExitCommand
	{
		
	public:
		
		static int type() { return LEFT_DOUBLE_CLICK; };
		
		void Execute(const Message& message)
		{
			CGEventRef ourEvent = CGEventCreate(NULL);
			CGPoint point = CGEventGetLocation(ourEvent);
			
			CGEventRef theEvent = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, point, kCGMouseButtonLeft);  
			CGEventSetIntegerValueField(theEvent, kCGMouseEventClickState, 2); 
			CGEventPost(kCGHIDEventTap, theEvent);  
			CGEventSetType(theEvent, kCGEventLeftMouseUp);  
			CGEventPost(kCGHIDEventTap, theEvent);  
			CGEventSetType(theEvent, kCGEventLeftMouseDown);  
			CGEventPost(kCGHIDEventTap, theEvent);  
			CGEventSetType(theEvent, kCGEventLeftMouseUp); 
			CGEventPost(kCGHIDEventTap, theEvent); 
			CFRelease(theEvent);
		};
		
	};


#endif