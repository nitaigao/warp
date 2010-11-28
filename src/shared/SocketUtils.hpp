class SocketUtils
{
  public:

    static void set_no_delay(int sock)
    {
        int no_delay = 1;
        setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&no_delay, sizeof(no_delay));	
    }

    static std::string hostname()
    {
        struct addrinfo hints, *info;
        int gai_result;
    
        char hostname[1024];
        hostname[1023] = '\0';
        gethostname(hostname, 1023);
    
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC; /*either IPV4 or IPV6*/
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_CANONNAME;
    
        if ((gai_result = getaddrinfo(hostname, "http", &hints, &info)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(gai_result));
            exit(1);
        }
    
        return info->ai_next->ai_canonname;
    }

    static void set_no_sigpipe(int sock)
    {
        fd_set outgoing; 
        FD_ZERO(&outgoing); 
        FD_SET(sock, &outgoing); 	
        setsockopt(sock, SOL_SOCKET, SO_NOSIGPIPE, (void *)&outgoing, sizeof(int));
    }

    static int open_socket(int type)
    {    
        int sock = socket(AF_INET, type, 0);
		
        if (sock < 0)
        {
            std::cerr << "ERROR opening socket" << std::endl;
        }
		
        return sock;
    }
	
    static void bind_socket(int sock, unsigned int port)
    {
        struct sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(port);
		
        if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        {
            std::cerr << "ERROR on binding: " << strerror(errno) << std::endl;
        }
    }		

    static void set_multicast_loopback(int sock, bool enabled)
    {
        u_char loopback = 0;
        if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, &loopback, sizeof(loopback)) < 0)
        {
            std::cerr << "ERROR setting multicast loopback" << std::endl;
        }
    }
	
    static void set_multicast(int sock, std::string address)
    {
        struct ip_mreq mreq;
        mreq.imr_multiaddr.s_addr = inet_addr(address.c_str());
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
            std::cerr << "ERROR setting up multicast" << std::endl;
        }
    }

    static void set_non_blocking(int sock)
    {
        int flags;  
        if ((flags = fcntl(sock, F_GETFL, 0)) < 0) 
        {
            std::cerr << "ERROR, couldn't get socket flags" << std::endl;
        }
		
        if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0) 
        { 
            std::cerr << "ERROR, couldn't set socket flags" << std::endl;
        }
    };

    static void set_reuse_port(int sock)
    {
        u_int yes = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
            std::cerr << "ERROR reusing ADDR" << std::endl;
        }
    }
};
