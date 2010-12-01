#include "ISocket.hpp"

class MultiSocket : public ISocket
{
  
public:
  
  MultiSocket(unsigned int port)
    : port_(port) { };
  
  void terminate();
  
  received_data* receive();
  
  void send(const char* data, unsigned int size);
  
  void listen_on();
  
  void dispose(received_data* data);
  
private:
  
  int max_socket_;
  int listen_sock_;
  
  unsigned int port_;
  fd_set read_sockets_;
  
};
