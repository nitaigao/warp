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

class BlackHole
{	
	
public:
	
	BlackHole(Client* client) : client_(client) { };
	
	//: client_(client), allowed_to_send_(false) { };
	
	void send_input() { client_->send_input(); };
	
	void attach()
	{
		CFMachPortRef      eventTap;
		CFRunLoopSourceRef runLoopSource;
		
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
		
		eventTap = CGEventTapCreate(kCGSessionEventTap,  kCGHeadInsertEventTap,  0, eventType, BlackHole::eventcallback,  &client_);
		
		if(!eventTap)
		{
			std::cerr << "failed to create event tap" << std::endl;
			exit(1);
		}
		
		runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
		CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
		CGEventTapEnable(eventTap, true);
		//CFRunLoopRun();
	}
	
private:
	
	Client* client_;
	
	static CGEventRef eventcallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon)
	{
		Client* client = (Client*)refcon;
		
		switch (type) {
				
			case kCGEventKeyDown:
			{
				std::clog << "key down" << std::endl;
				CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
				client->send_key_down(keycode);
				NSLog(@"key down %d", keycode);
				break;
			}
				
			case kCGEventKeyUp:
			{
				std::clog << "key up" << std::endl;
				
				break;
			}
				
			case kCGEventFlagsChanged:
			{
				std::clog << "flags changed" << std::endl;
				/*int keycode = [theEvent keyCode];
				unsigned int flags = [theEvent modifierFlags];
				client->send_flags(keycode, flags);*/					
				break;
			}
				
			case kCGEventLeftMouseUp:
			{
				std::clog << "left mouse up" << std::endl;
				client->send_left_up();
				break;
			}
				
			case kCGEventLeftMouseDown:
			{
				std::clog << "left mouse down" << std::endl;
				client->send_left_down();
				break;
			}
				
			case kCGEventLeftMouseDragged:
			{
				std::clog << "left mouse dragged" << std::endl;
				/*float x = [theEvent deltaX];
				float y = [theEvent deltaY];
				client->send_left_dragged(x, y);*/
				break;
			}
				
			case kCGEventRightMouseUp:
			{
				std::clog << "right mouse up" << std::endl;
				client->send_right_up();
				break;
			}
				
			case kCGEventRightMouseDown:
			{
				std::clog << "right mouse down" << std::endl;
				client->send_right_down();
				break;
			}
				
			case kCGEventRightMouseDragged:
			{
				std::clog << "right mouse dragged" << std::endl;
				/*float x = [theEvent deltaX];
				float y = [theEvent deltaY];
				client->send_right_dragged(x, y);*/
				break;
			}
				
			case kCGEventMouseMoved:
			{
				std::clog << "mouse moved" << std::endl;
				/*float x = [theEvent deltaX];
				float y = [theEvent deltaY];
				client->send_mouse_moved(x, y);	*/
				break;
			}
				
			case kCGEventScrollWheel:
			{
				std::clog << "scroll wheel" << std::endl;
				/*float x = [theEvent deltaX];
				float y = [theEvent deltaY];
				client->send_scroll_wheel(x, y);*/
				break;
			}
		}
		
		return event;
	}
};