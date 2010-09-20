/*
 *  Client.h
 *  warp
 *
 *  Created by Nicholas Kostelnik on 11/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include "Message.h"
#include <string>

class Client
{
	
public:
	
	Client() : connected_(false) { };
	
	void send_input() { connected_ = true; };
	
	bool connected() { return connected_; };
	
	bool connec(const std::string& host, unsigned int port);
	
	void reconnect();
	
	void disconnect();
	
	int send_left_up();
	
	int send_right_up();
	
	int send_left_down();
	
	int send_left_dragged(int x, int y);
	
	int send_right_dragged(int x, int y);
	
	int send_right_down();	
	
	int send_mouse_moved(int x, int y);
	
	int send_key_down(unsigned int flags, int key_code);
	
	int send_key_up(unsigned int flags, int key_code);
	
	int send_flags(int key_code, unsigned int flags);
	
	int send_scroll_wheel(int x, int y);
	
	int send_left_double_click();
	
private:
	
	int send_message(const Message& message);
	
	int sock;
	
	std::string last_host_;
	
	bool connected_;
};