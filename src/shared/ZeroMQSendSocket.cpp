#include "ZeroMQSendSocket.h"

#include <zmq.hpp>
#include <sstream>
#include <iostream>
#include <string>

#include "ZeroMQContext.hpp"

std::string ZeroMQSendSocket::final_host(const std::string& host, unsigned int port)
{
  std::stringstream final_host;
  final_host << "tcp://" << host << ":" << port;
  return final_host.str();
};

ZeroMQSendSocket::ZeroMQSendSocket() 
  : socket_(0) 
{ 

};


ZeroMQSendSocket::~ZeroMQSendSocket()
{
  terminate();
}

bool ZeroMQSendSocket::connect_to(const std::string& host, unsigned int port)
{
  
//  terminate();
  socket_ = ZeroMQContext::instance()->create_socket(ZMQ_PUSH); 

  try {
    socket_->connect(final_host(host, port).c_str());
  }
  catch (zmq::error_t e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return true;
};

void ZeroMQSendSocket::terminate()
{
  if (socket_ != 0)
  {
#ifdef _WIN32
    Sleep(1);
#else
	sleep(1);
#endif
    std::clog << "closing connection" << std::endl;
    try {
      delete socket_;
      socket_ = 0;
    }
    catch (zmq::error_t e) {
      std::cerr << e.what() << std::endl;
    }
    std::clog << "closed connection" << std::endl;
  }
};

bool ZeroMQSendSocket::send(void *data, size_t data_size)
{
  zmq::message_t message(data_size);
  memcpy(message.data(), data, data_size);
  return socket_->send(message);
};