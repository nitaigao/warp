#include "MultiCastSocket.h"

#include <iostream>

#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>

#include "SocketUtils.hpp"

const static int BUFFER_SIZE = 256;
#define WORMHOLE_GROUP "225.0.0.87"

void MultiSocket::terminate() 
{
  close(listen_sock_);
}

ISocket::received_data* MultiSocket::receive()
{
  struct timeval timeout;
  timeout.tv_sec = 1;
  timeout.tv_usec = 1;
  
  fd_set working_socks;
  FD_ZERO(&working_socks);
  memcpy(&working_socks, &read_sockets_, sizeof(read_sockets_));
  
  int readsocks = select(max_socket_ + 1, &working_socks, NULL, NULL, &timeout);
  
  if (readsocks < 0)
  { 
    std::cerr << "ERROR on accept" << std::endl;
  }
  
  received_data* return_data = new received_data();
  
  if (readsocks > 0)
  {      
    for (int i = listen_sock_; i < max_socket_ + 1; i++)
    {      
      if (FD_ISSET(i, &working_socks))
      {
        if (i == listen_sock_)
        {
          char* buffer = new char[BUFFER_SIZE];
          memset(buffer, 0, BUFFER_SIZE);
          
          int result = read(i, buffer, BUFFER_SIZE);
          
          if (result < 0)
          { 
            std::cerr << "ERROR reading from socket: " << strerror(errno) << std::endl;
          }
          
          if (result == 0)
          {
            close(i);
            shutdown(i, 5);
            FD_CLR(i, &read_sockets_);
          }
          
          if (result > 0)
          {
            return_data->push_back(buffer);
          }
        }
      }
    }
  }
  
  return return_data;
}

void MultiSocket::send(const char* data, unsigned int size) 
{
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(WORMHOLE_GROUP);
  addr.sin_port = htons(port_);
  
  if (sendto(listen_sock_, data, size, 0, (struct sockaddr *) &addr, sizeof(addr)) < 0)
  {
    std::cerr << "ERROR sending multicast" << std::endl; 
  }
}

void MultiSocket::listen_on()
{
  listen_sock_ = SocketUtils::open_socket(SOCK_DGRAM);
  SocketUtils::set_non_blocking(listen_sock_);
  SocketUtils::set_reuse_port(listen_sock_);
  
  SocketUtils::bind_socket(listen_sock_, port_);
  SocketUtils::set_multicast(listen_sock_, WORMHOLE_GROUP);
  SocketUtils::set_multicast_loopback(listen_sock_, false);
  
  max_socket_ = listen_sock_;
  FD_ZERO(&read_sockets_);  
  FD_SET(listen_sock_, &read_sockets_);
  listen(listen_sock_, 32);
  
};

void MultiSocket::dispose(received_data* data)
{
  for (received_data::iterator i = data->begin(); i != data->end(); ++i)
  {
    delete[] (*i);
  }
  
  delete data;
}
