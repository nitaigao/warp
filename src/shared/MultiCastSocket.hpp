#define WORMHOLE_GROUP "225.0.0.37"

class MultiSocket : public ISocket
{

  public:

  MultiSocket(unsigned int port)
      : port_(port) { };

    received_data* receive()
    {
        struct sockaddr_in addr;  
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(port_);
        socklen_t addrlen = sizeof(addr);
        
        received_data* return_data = new received_data();

        char* buffer = new char[256];
      
        if (recvfrom(multicast_sock_, buffer, sizeof(buffer), 0, (struct sockaddr *) &addr, &addrlen) < 0)
        {  
            if (errno != EWOULDBLOCK)
            {
                std::cerr << "ERROR reading from multicast group" << std::endl;
            }                   
        }
        else 
        {            
            return_data->push_back(buffer);
        }

        return return_data; 
    }

    void send(const char* data, unsigned int size) 
    {
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(WORMHOLE_GROUP);
        addr.sin_port = htons(port_);

        sendto(multicast_sock_, data, size, 0, (struct sockaddr *) &addr, sizeof(addr));
    }
		
    void listen_on()
    {
        multicast_sock_ = SocketUtils::open_socket(SOCK_DGRAM);
        SocketUtils::set_non_blocking(multicast_sock_);
        SocketUtils::set_reuse_port(multicast_sock_);
        SocketUtils::bind_socket(multicast_sock_, port_);    
        SocketUtils::set_multicast(multicast_sock_, WORMHOLE_GROUP);
        SocketUtils::set_multicast_loopback(multicast_sock_, false);
    };

    void dispose(received_data* data)
    {
        for (received_data::iterator i = data->begin(); i != data->end(); ++i)
        {
            delete[] (*i);
        }
		
        delete data;
    }
		
  private:

    unsigned int port_;
    int multicast_sock_;

};
