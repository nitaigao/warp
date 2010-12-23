#include <string>


extern "C" {
  #include <pgm/pgm.h>
  #include <pgm/in.h>
}

class Multicast
{
  
public:
  
  Multicast();
  ~Multicast();
  
  void broadcast();
  
  std::string receive();
  
private:
  
  pgm_sock_t* socket_;
  
};