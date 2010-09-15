/*
 *  IServerCommand.h
 *  warp
 *
 *  Created by Nicholas Kostelnik on 15/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

class IServerCommand
{
	
public:
	
	virtual void Execute(const Message& message) = 0;
	
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
	
};

class LeftUpCommand : public IServerCommand
{
	
public:
	
	static int type() { return LEFT_UP; };
	
	void Execute(const Message& message) 
	{ 
		std::clog << "left up" << std::endl;
		
		CGEventRef event = CGEventCreate(NULL);
		CGPoint point = CGEventGetLocation(event); 
		PostMouseEvent(kCGMouseButtonLeft, kCGEventLeftMouseUp, point);
	};
	
};

class LeftDownCommand : public IServerCommand
{
	
public:
	
	static int type() { return LEFT_DOWN; };
	
	void Execute(const Message& message) 
	{ 
		std::clog << "left down" << std::endl;
		
		CGEventRef event = CGEventCreate(NULL);
		CGPoint point = CGEventGetLocation(event); 
		PostMouseEvent(kCGMouseButtonLeft, kCGEventLeftMouseDown, point);
	};
	
};


class RightUpCommand : public IServerCommand
{
	
public:
	
	static int type() { return RIGHT_UP; };
	
	void Execute(const Message& message) 
	{ 
		std::clog << "right up" << std::endl;
		
		CGEventRef event = CGEventCreate(NULL);
		CGPoint point = CGEventGetLocation(event); 
		PostMouseEvent(kCGMouseButtonRight, kCGEventRightMouseUp, point);
	};
	
};

class RightDownCommand : public IServerCommand
{
	
public:
	
	static int type() { return RIGHT_DOWN; };
	
	void Execute(const Message& message)
	{
		std::clog << "right down" << std::endl;
		
		CGEventRef event = CGEventCreate(NULL);
		CGPoint point = CGEventGetLocation(event); 
		PostMouseEvent(kCGMouseButtonRight, kCGEventRightMouseDown, point);
	};
};

class KeyUpCommand : public IServerCommand
{
	
public:
	
	static int type() { return KEY_UP; };
	
	void Execute(const Message& message)
	{
		std::clog << "key up" << std::endl;
		
		CGEventRef e = CGEventCreateKeyboardEvent (NULL, message.key_code, true);
		CGEventSetFlags(e, (CGEventFlags)message.flags);
		CGEventPost(kCGSessionEventTap, e);
		CFRelease(e);
		
	};
	
};

class KeyDownCommand : public IServerCommand
{
	
public:
	
	static int type() { return KEY_DOWN; };
	
	void Execute(const Message& message)
	{
		std::clog << "key down" << std::endl;
		
		CGEventRef e = CGEventCreateKeyboardEvent (NULL, message.key_code, true);
		CGEventSetFlags(e, (CGEventFlags)message.flags);
		CGEventPost(kCGSessionEventTap, e);
		CFRelease(e);
	};
	
};

class MouseMovedCommand : public IServerCommand
{
	
public:
	
	static int type() { return MOUSE_MOVED; };
	
	void Execute(const Message& message)
	{
		std::clog << "mouse moved" << std::endl;
		
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

class LeftDraggedCommand : public IServerCommand
{
	
public:
	
	static int type() { return LEFT_DRAGGED; };
	
	void Execute(const Message& message) 
	{
		std::clog << "left dragged" << std::endl;
		
		CGEventRef ourEvent = CGEventCreate(NULL);
		CGPoint epoint = CGEventGetLocation(ourEvent);
		
		CGPoint point;
		point.x = epoint.x + message.x;
		point.y = epoint.y + message.y;
		PostMouseEvent(kCGMouseButtonLeft, kCGEventLeftMouseDragged, point);
	};
	
};

class RightDraggedCommand : public IServerCommand
{
	
public:
	
	static int type() { return RIGHT_DRAGGED; };
	
	void Execute(const Message& message)
	{
		std::clog << "right dragged" << std::endl;
		
		CGEventRef ourEvent = CGEventCreate(NULL);
		CGPoint epoint = CGEventGetLocation(ourEvent);
		
		CGPoint point;
		point.x = epoint.x + message.x;
		point.y = epoint.y + message.y;
		PostMouseEvent(kCGMouseButtonRight, kCGEventRightMouseDragged, point);
	};
	
};

class FlagsChangedCommand : public IServerCommand
{
	
public:
	
	static int type() { return FLAGS_CHANGED; };
	
	void Execute(const Message& message)
	{
		std::clog << "flags changed" << std::endl;
		
		CGEventRef e = CGEventCreateKeyboardEvent (NULL, message.key_code, true);
		CGEventSetFlags(e, (CGEventFlags)message.flags);
		CGEventPost(kCGSessionEventTap, e);
		CFRelease(e);
	};
	
};

class ScrollWheelCommand : public IServerCommand
{
	
public:
	
	static int type() { return SCROLL_WHEEL; };
	
	void Execute(const Message& message)
	{
		std::clog << "scroll wheel" << std::endl;
		
		CGEventRef ourEvent = CGEventCreate(NULL);
		CGPoint point = CGEventGetLocation(ourEvent);
		
		CGEventRef e = CGEventCreateScrollWheelEvent(NULL, kCGScrollEventUnitPixel, 1, 2);
		
		CGEventSetType(e, kCGEventScrollWheel);
		CGEventSetIntegerValueField(e, kCGScrollWheelEventDeltaAxis1, message.y);
		CGEventPost(kCGSessionEventTap, e);
		CFRelease(e);
	};
	
};

class LeftDoubleClickCommand : public IServerCommand
{
	
public:
	
	static int type() { return LEFT_DOUBLE_CLICK; };
	
	void Execute(const Message& message)
	{
		std::clog << "left double click" << std::endl;
		
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