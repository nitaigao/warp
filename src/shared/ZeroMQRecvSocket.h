#ifndef ZEROMQRECVSOCKET_HPP
#define ZEROMQRECVSOCKET_HPP

  #include "IRecvSocket.hpp"

  namespace zmq { class socket_t; };

  class ZeroMQRecvSocket : public IRecvSocket
  {
    
  public:
    
    ZeroMQRecvSocket();
    
    Message* receive();
    
    void dispose(Message* message);
    
    void terminate();
    
  private:
    
    zmq::socket_t* socket_;
    
  };

#endif
