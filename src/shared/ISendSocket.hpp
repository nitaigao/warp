#ifndef ISENDSOCKET_HPP
#define ISENDSOCKET_HPP

  #include <string> 

  class ISendSocket
  {
    
  public:
    
    virtual bool connect_to(const std::string& host, unsigned int port) = 0;
    
    virtual void terminate() = 0;
    
    virtual bool send(void *data, size_t data_size) = 0;
    
  };

#endif