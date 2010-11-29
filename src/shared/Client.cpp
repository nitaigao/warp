/*
 *  Client.cpp
 *  warp
 *
 *  Created by Nicholas Kostelnik on 11/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Client.h"
#include "Constants.hpp"

void Client::update(float delta)
{
  if (connected_ && timeout_ > 0)
  {
    timeout_ -= delta;
  
    if (timeout_ <= 0)
    {
      disconnect();
    }
  }
  
  ISocket::received_data* datas = m_recv_socket_->receive();
  
  for (ISocket::received_data::iterator data = datas->begin(); data != datas->end(); ++data) 
  {	
    std::string host = (*data);
    new_known_hosts_.push_back(host);
  }
  
  m_recv_socket_->dispose(datas);
}

bool Client::connect_to(const std::string& host, unsigned int port)
{	
	bool result = static_cast<TCPSocket*>(socket_)->connect_to(host, port);
	timeout_ = TIME_OUT;
	connected_ = true;
	last_host_ = host;
	return result;
}

bool Client::reconnect()
{
	bool result = false;
	
	if (can_reconnect())
	{
		result = this->connect_to(last_host_, SERVER_PORT);
	}
	
	return result;
}

StringList Client::known_hosts()
{
  StringList hosts = new_known_hosts_;
  new_known_hosts_.clear();
  return hosts;
}

void Client::search_for_hosts()
{
  std::string message = "advertise";
  m_send_socket_->send(message.c_str(), message.length()); //1724
}

void Client::disconnect()
{
	connected_ = false;
  static_cast<TCPSocket*>(socket_)->terminate();
}

bool Client::can_reconnect()
{
	return (last_host_.length() > 0);
}

void Client::send_message(const Message& message)
{	
	if (!connected_)
	{
		reconnect();
	}
	
	timeout_ = TIME_OUT;
	char* data = new char[sizeof(Message)];
	memcpy(data, &message, sizeof(Message));
	
	socket_->send(data, sizeof(Message));
}

void Client::send_left_double_click()
{
	Message message;
	message.type = LEFT_DOUBLE_CLICK;
	send_message(message);
}

void Client::send_left_down()
{
	Message message;
	message.type = LEFT_DOWN;
	send_message(message);
}

void Client::send_key_down(unsigned int flags, int key_code)
{
	Message message;
	message.type = KEY_DOWN;
	message.key_code = key_code;
	message.flags = flags;
	send_message(message);
}

void Client::send_key_up(unsigned int flags, int key_code)
{
	Message message;
	message.type = KEY_UP;
	message.key_code = key_code;
	message.flags = flags;
	send_message(message);
}

void Client::send_right_down()
{
	Message message;
	message.type = RIGHT_DOWN;
	send_message(message);
}

void Client::send_left_up()
{
	Message message;
	message.type = LEFT_UP;
	send_message(message);
}

void Client::send_right_up()
{
	Message message;
	message.type = RIGHT_UP;
	send_message(message);
}

void Client::send_mouse_moved(int x, int y)
{
	Message message;
	message.type = MOUSE_MOVE;
	message.x = x;
	message.y = y;
	send_message(message);
}

void Client::send_left_dragged(int x, int y)
{
	Message message;
	message.type = LEFT_DRAGGED;
	message.x = x;
	message.y = y;
	send_message(message);
}

void Client::send_right_dragged(int x, int y)
{
	Message message;
	message.type = RIGHT_DRAGGED;
	message.x = x;
	message.y = y;
	send_message(message);
}

void Client::send_flags(int key_code, unsigned int flags)
{
	Message message;
	message.type = FLAGS_CHANGED;
	message.key_code = key_code;
	message.flags = flags;
	send_message(message);
}

void Client::send_scroll_wheel(int x, int y)
{
	Message message;
	message.type = SCROLL_WHEEL;
	message.x = x;
	message.y = y;	
	send_message(message);
}