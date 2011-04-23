#include "ZeroMQRecvSocket.h"

#include <zmq.hpp>
#include <iostream>
#include <sstream>

#include "IRecvSocket.hpp"
#include "ZeroMQContext.hpp"
#include "Constants.hpp"

ZeroMQRecvSocket::ZeroMQRecvSocket()
{
  socket_ = ZeroMQContext::instance()->create_socket(ZMQ_PULL);
  std::stringstream final_host;
  final_host << "tcp://*:" << SERVER_PORT;
  
  try {
      socket_->bind(final_host.str().c_str());
  }
  catch (zmq::error_t e) {
    std::cerr << e.what() << std::endl;
  }
}	

Message* ZeroMQRecvSocket::receive()
{
  zmq::message_t message;
  
  try {
    socket_->recv(&message);
  }
  catch (zmq::error_t e) {
    std::cerr << e.what() << std::endl;
  }
  
  Message* data = new Message();
  memcpy(data, message.data(), message.size());
  return data;
};

void ZeroMQRecvSocket::dispose(Message* message)
{
  delete message;
}

void ZeroMQRecvSocket::terminate()
{
  delete socket_;
};