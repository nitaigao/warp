#ifndef ZEROMQCONTEXT_HPP
#define ZEROMQCONTEXT_HPP

#include <zmq.hpp>

class ZeroMQContext
{
  static ZeroMQContext* instance_;
  zmq::context_t* context_;
  
  ZeroMQContext();
  
public:
  
  static void init();
  
  static void destroy();
  
  static ZeroMQContext* instance();
  
  zmq::socket_t* create_socket(int type);

};

#endif