#include "Exit.h"

#include "Message.h"
#include "IExitCommand.hpp"
#include "SocketUtils.hpp"

#ifdef _WIN32

#include "WinExitCommands.hpp"
#include "WinSocket.h"

Exit::Exit(unsigned int port) 
: socket_(new WinSocket())
{
		message_types_[MOUSE_MOVE]				= new MouseMovedCommand();
		message_types_[LEFT_UP]					= new LeftUpCommand();
		message_types_[LEFT_DOWN]				= new LeftDownCommand();
		message_types_[RIGHT_UP]				= new RightUpCommand();
		message_types_[RIGHT_DOWN]				= new RightDownCommand();
		message_types_[LEFT_DOUBLE_CLICK]		= new LeftDoubleClickCommand();
		message_types_[KEY_UP]					= new KeyUpCommand();
		message_types_[KEY_DOWN]				= new KeyDownCommand();
}

#else

#include "OSXExitCommands.hpp"
#include "TCPSocket.h"

Exit::Exit(ISocket* send_socket, ISocket* m_recv_socket, ISocket* m_send_socket) 
  : send_socket_(send_socket) 
  , m_send_socket_(m_send_socket)
  , m_recv_socket_(m_recv_socket)
{		
		message_types_[LEFT_UP]						= new LeftUpCommand();
		message_types_[LEFT_DOWN]					= new LeftDownCommand();
		message_types_[RIGHT_UP]					= new RightUpCommand();
		message_types_[RIGHT_DOWN]				= new RightDownCommand();
		message_types_[KEY_UP]						= new KeyUpCommand();
		message_types_[KEY_DOWN]					= new KeyDownCommand();
		message_types_[MOUSE_MOVE]				= new MouseMovedCommand();
		message_types_[LEFT_DRAGGED]			= new LeftDraggedCommand();
		message_types_[RIGHT_DRAGGED]			= new RightDraggedCommand();
		message_types_[FLAGS_CHANGED]			= new FlagsChangedCommand();
		message_types_[SCROLL_WHEEL]			= new ScrollWheelCommand();
		message_types_[LEFT_DOUBLE_CLICK]	= new LeftDoubleClickCommand();
}

#endif

void Exit::receive_search() 
{
  ISocket::received_data* listen_datas = m_recv_socket_->receive();
  
  if (!listen_datas->empty())
  {
    std::string hostname = SocketUtils::hostname();
    m_send_socket_->send(hostname.c_str(), hostname.size());
  }
  
  m_recv_socket_->dispose(listen_datas);  
}
	
void Exit::receive_input() 
{ 
    ISocket::received_data* datas = send_socket_->receive();
	
    for (ISocket::received_data::iterator i = datas->begin(); i != datas->end(); ++i) 
    {			
        Message message;
        memcpy(&message, (*i), sizeof(Message));
  
        if (message_types_.find(message.type) != message_types_.end())
        {							
            message_types_[message.type]->Execute(message);
        }
    }
  
  send_socket_->dispose(datas);
};

void Exit::shutdown()
{
  send_socket_->terminate();
}
