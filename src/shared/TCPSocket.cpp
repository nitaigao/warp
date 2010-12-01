#include "TCPSocket.h"

#include "SocketUtils.hpp"

const static int BUFFER_SIZE = 256;

bool TCPSocket::connect_to(const std::string& host, unsigned int port)
{
  struct hostent *server = gethostbyname(host.c_str());
  
  if (server == NULL)
  {
    std::cerr << "ERROR, no such host" << std::endl;
    return false;
  }
  
  std::clog << "connecting to: " << host << std::endl;
  
  socket_ = SocketUtils::open_socket(SOCK_STREAM);
  
  if (socket_ < 0)
  {
    std::cerr << "ERROR, unable to create socket" << std::endl;
    return false;
  }
  
  SocketUtils::set_no_delay(socket_);
  SocketUtils::set_non_blocking(socket_);
  
  fd_set outgoing; 
  FD_ZERO(&outgoing); 
  FD_SET(socket_, &outgoing); 
  
  setsockopt(socket_, SOL_SOCKET, SO_NOSIGPIPE, (void *) &outgoing, sizeof(int));
  
  struct sockaddr_in server_address;
  bzero((char *) &server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  bcopy((char *) server->h_addr, (char *) &server_address.sin_addr.s_addr, server->h_length);
  server_address.sin_port = htons(port);
  
  connect(socket_, (struct sockaddr*)&server_address, sizeof(server_address));
  
  struct timeval tv; 
  tv.tv_sec = 3; 
  tv.tv_usec = 0; 
  
  int result = select(socket_ + 1, NULL, &outgoing, NULL, &tv);
  
  if (result > 0)
  {
    socklen_t lon = sizeof(int);
    int valopt;
    
    if (getsockopt(socket_, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0) 
    { 
      std::cerr << "ERROR, can't get socket options" << std::endl; 
    } 
    else if (valopt) 
    { 
      std::cerr << "ERROR, connection refused" << std::endl;
    }
    else 
    {
      std::clog << "connected" << std::endl;
      return true;
    }
  }
  
  return false;
}
  
void TCPSocket::terminate()
{
  close(socket_);
  shutdown(socket_, 1);	
}

ISocket::received_data* TCPSocket::receive()
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
          int incoming_socket = accept(listen_sock_, NULL, NULL);
          
          if (incoming_socket < 0)
          {							
            std::cerr << "ERROR accepting incoming connection: " << strerror(errno) << std::endl;
          }
          
          FD_SET(incoming_socket, &read_sockets_);
          max_socket_ = incoming_socket;
        }
        else
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

void TCPSocket::send(const char* data, unsigned int size)
{		
  int result = write(socket_, data, size);
  
  if (result < 0)
  {
    std::cerr << "ERROR writing to socket" << std::endl;
  }
}

void TCPSocket::listen_on()
{
  listen_sock_ = SocketUtils::open_socket(SOCK_STREAM);
  SocketUtils::set_non_blocking(listen_sock_);
  SocketUtils::set_reuse_port(listen_sock_);
  SocketUtils::bind_socket(listen_sock_, port_);
  
  max_socket_ = listen_sock_;
  FD_ZERO(&read_sockets_);  
  FD_SET(listen_sock_, &read_sockets_);
  listen(listen_sock_, 32);
}

void TCPSocket::dispose(received_data* data)
{
  for (received_data::iterator i = data->begin(); i != data->end(); ++i)
  {
    delete[] (*i);
  }
  
  delete data;
}