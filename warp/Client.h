/*
 *  Client.h
 *  warp
 *
 *  Created by Nicholas Kostelnik on 11/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include "Message.h"

class Client
{
	
public:
	
	Client() { };
	
	void attach();
	
	void send_left_up();
	
	void send_right_up();
	
	void send_left_down();
	
	void send_left_dragged(int x, int y);
	
	void send_right_dragged(int x, int y);
	
	void send_right_down();	
	
	void send_mouse_moved(int x, int y);
	
	void send_key_down(int key_code);
	
	void send_flags(int key_code, unsigned int flags);
	
	void send_scroll_wheel(int x, int y);
	
	void send_left_double_click();
	
private:
	
	void send_message(const Message& message);
	
	int sock;
};