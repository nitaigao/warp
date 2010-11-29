#include "Entrance.h"

Entrance::Entrance(Client* client, NSWindow* window) : client_(client), window_(window), enabled_(false)
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

bool Entrance::send_to(const std::string& host, unsigned int port)
{
	bool result = client_->connect_to(host, port);
	
	if (result)
	{
		enable();
	}
	
	return result;
}

void Entrance::update(float delta)
{
  client_->update(delta);
}


void Entrance::tap()
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
	
	event_tap_ = CGEventTapCreate(kCGSessionEventTap,  kCGHeadInsertEventTap,  0, eventType, Entrance::event_tap, this);
	
	if(!event_tap_)
	{
		std::cerr << "failed to create event tap" << std::endl;
	}
	
	CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, event_tap_, 0);
	CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
	CGEventTapEnable(event_tap_, true);
}

void Entrance::disable()
{
	CGDisplayShowCursor(kCGDirectMainDisplay);
	CGAssociateMouseAndMouseCursorPosition(true);
	[window_ orderOut:nil];
	client_->disconnect();
	enabled_ = false;
}

void Entrance::search_for_exits()
{
  client_->search_for_hosts();
}

StringList Entrance::network_hosts()
{
  return client_->known_hosts();
}

CGEventRef Entrance::event_tap(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon)
{
	Entrance* black_hole = (Entrance*)refcon;
	return black_hole->on_event(type, event);
}

void Entrance::enable() 
{ 
	CGAssociateMouseAndMouseCursorPosition(false);
	CGDisplayHideCursor(kCGDirectMainDisplay);
	[window_ makeKeyAndOrderFront:nil];
	[[NSApplication sharedApplication] activateIgnoringOtherApps : YES];
	enabled_ = true;
};

CGEventRef Entrance::on_event(CGEventType type, CGEventRef event)
{			
	scan_reconnect(type, event);
	return scan_input(type, event);
}

void Entrance::scan_reconnect(CGEventType type, CGEventRef event)
{
	if (type == kCGEventKeyDown) 
	{
		CGEventFlags flags = CGEventGetFlags(event);
		CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
		
		if ((flags & kCGEventFlagMaskShift) && (flags & kCGEventFlagMaskCommand) && keycode == 14) // cmd-shift-e
		{
			if (enabled_) 
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

CGEventRef Entrance::scan_input(CGEventType type, CGEventRef event)
{	
	if (enabled_) 
	{		
		if (type == kCGEventTapDisabledByTimeout)
		{
			tap();
		}

		if (client_commands_.find(type) != client_commands_.end())
		{
      try
      {
          client_commands_[type]->Execute(event, client_);
      }
      catch(std::exception e)
      {
          std::cerr << e.what() << std::endl;
      }
			event = NULL;
		}
	}
	
	return event;		
}
