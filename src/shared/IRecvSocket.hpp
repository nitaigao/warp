#ifndef IRECVSOCKET_HPP
#define IRECVSOCKET_HPP

  #include "Message.h" 

  class IRecvSocket
  {
    
  public:
    
    virtual Message* receive() = 0;
    
    virtual void dispose(Message* message) = 0;
    
    virtual void terminate() = 0;
    
  };

#endif