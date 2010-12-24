#include "Exit.h"

#include "Message.h"
#include "ZeroMQRecvSocket.h" 

#include "IExitCommand.hpp"
#ifdef _WIN32
#include "WinExitCommands.hpp"
#else
#include "OSXExitCommands.hpp"
#endif

Exit::Exit() {
#ifndef _WIN32
  message_types_[LEFT_DRAGGED]			= new LeftDraggedCommand();
  message_types_[RIGHT_DRAGGED]			= new RightDraggedCommand();
  message_types_[FLAGS_CHANGED]			= new FlagsChangedCommand();
  message_types_[SCROLL_WHEEL]			= new ScrollWheelCommand();
#endif
  message_types_[LEFT_UP]           = new LeftUpCommand();
  message_types_[LEFT_DOWN]         = new LeftDownCommand();
  message_types_[RIGHT_UP]          = new RightUpCommand();
  message_types_[RIGHT_DOWN]				= new RightDownCommand();
  message_types_[KEY_UP]            = new KeyUpCommand();
  message_types_[KEY_DOWN]          = new KeyDownCommand();  
  message_types_[MOUSE_MOVE]				= new MouseMovedCommand();
  message_types_[LEFT_DOUBLE_CLICK]	= new LeftDoubleClickCommand();  

  exit_socket_ = new ZeroMQRecvSocket();
}

void Exit::receive_search() 
{
  
}

#include <sstream>
	
void Exit::receive_input() 
{ 
  Message* message = exit_socket_->receive();

  if (message_types_.find(message->type) != message_types_.end())
  {							
    message_types_[message->type]->Execute(*message);
  }
  
  exit_socket_->dispose(message);      
};

void Exit::shutdown()
{
  exit_socket_->terminate();
}
