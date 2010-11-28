#ifndef CLIENT_H
#define CLIENT_H

	#include "Message.h"
	#include <string>

	#include "TCPSocket.h"

	class Client
	{
		
	public:
		
		Client() : connected_(false), timeout_(0), socket_(new TCPSocket(0)), last_host_("") { };
		
		bool connected() { return connected_; };
		
		bool connect_to(const std::string& host, unsigned int port);
		
		void update(float delta);
		
		bool reconnect();
		
		void disconnect();
		
		void send_left_up();
		
		void send_right_up();
		
		void send_left_down();
		
		void send_left_dragged(int x, int y);
		
		void send_right_dragged(int x, int y);
		
		void send_right_down();	
		
		void send_mouse_moved(int x, int y);
		
		void send_key_down(unsigned int flags, int key_code);
		
		void send_key_up(unsigned int flags, int key_code);
		
		void send_flags(int key_code, unsigned int flags);
		
		void send_scroll_wheel(int x, int y);
		
		void send_left_double_click();
		
	private:
		
		bool can_reconnect();
		
		void send_message(const Message& message);
		
		TCPSocket* socket_;
		
		std::string last_host_;
		
		float timeout_;
		
		bool connected_;
	};

#endif