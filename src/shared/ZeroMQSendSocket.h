#ifndef ZEROMQSENDSOCKET_HPP
#define ZEROMQSENDSOCKET_HPP

  #include "ISendSocket.hpp"

  namespace zmq { class socket_t; };

  class ZeroMQSendSocket : public ISendSocket
  {
    
  public:
    
    ZeroMQSendSocket();
    
    ~ZeroMQSendSocket();
    
    bool connect_to(const std::string& host, unsigned int port);
    
    void terminate();
    
    bool send(void *data, size_t data_size);
    
  private:
    
    std::string final_host(const std::string& host, unsigned int port);
    
    zmq::socket_t* socket_;
    
  };

#endif