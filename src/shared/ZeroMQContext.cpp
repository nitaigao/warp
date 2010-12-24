#include "ZeroMQContext.hpp"

ZeroMQContext* ZeroMQContext::instance_ = NULL;

ZeroMQContext::ZeroMQContext()
{
  context_ = new zmq::context_t(1);
};

void ZeroMQContext::init()
{
  instance_ = new ZeroMQContext();
};

void ZeroMQContext::destroy()
{
  delete instance_;
}

ZeroMQContext* ZeroMQContext::instance()
{
  return instance_;
}

zmq::socket_t* ZeroMQContext::create_socket(int type)
{
  return new zmq::socket_t(*context_, type);
}