#ifndef CLIENT_H
#define CLIENT_H

	#include "Message.h"
	#include <string>
  #include <vector>

  #include "ZeroMQSendSocket.h"

  typedef std::vector<std::string> StringList;  

	class Client
	{
		
	public:
		
    Client()
      : connected_(false)
      , timeout_(0)
      , last_host_("") 
    { 
      socket_ = new ZeroMQSendSocket();      
    };
    
		bool connected() { return connected_; };
		
		bool connect_to(const std::string& host, unsigned int port);
		
		void update_input(float delta);
    
    void update_search();
		
		bool reconnect();
		
		void disconnect();
		
		bool send_left_up();
		
		bool send_right_up();
		
		bool send_left_down();
		
		bool send_left_dragged(int x, int y);
		
		bool send_right_dragged(int x, int y);
		
		bool send_right_down();	
		
		bool send_mouse_moved(int x, int y);
		
		bool send_key_down(unsigned int flags, int key_code);
		
		bool send_key_up(unsigned int flags, int key_code);
		
		bool send_flags(int key_code, unsigned int flags);
		
		bool send_scroll_wheel(int x, int y);
		
		bool send_left_double_click();
    
    void search_for_hosts();
    
    StringList known_hosts();
		
	private:
		
		bool can_reconnect();
		
		bool send_message(const Message& message);
    
    ISendSocket* socket_;
		
		std::string last_host_;
    StringList new_known_hosts_;
		
		float timeout_;
		bool connected_;
	};

#endif