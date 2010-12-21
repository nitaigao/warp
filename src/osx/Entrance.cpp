#include "Entrance.h"

Entrance::Entrance()
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
  
  client_ = new Client();
};

bool Entrance::connect_to(const std::string& host, unsigned int port)
{
	bool result = client_->connect_to(host, port);
	
	if (result)
	{
		enable();
	}
	
	return result;
}

bool Entrance::understands(const CGEventType& event_type)
{
  if (!enabled_)
  {
    return false; 
  }
  
  return (client_commands_.find(event_type) != client_commands_.end());
}

void Entrance::on_event(CGEventType type, CGEventRef event)
{			  
  if (enabled_) 
  {
    scan_input(type, event);
  }
}

CGEventRef Entrance::scan_input(CGEventType type, CGEventRef event)
{	
	if (client_commands_.find(type) != client_commands_.end())
  {
    try
    {
      if (!client_commands_[type]->Execute(event, client_))
      {
        disable();
      }
      event = NULL;
    }
    catch(std::exception e)
    {
      std::cerr << e.what() << std::endl;
    }
  }
	
	return event;		
}

void Entrance::toggle()
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

void Entrance::disable()
{
  client_->disconnect();
	enabled_ = false;
}

void Entrance::enable() 
{ 
  enabled_ = true;
};