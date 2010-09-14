//gcc -o server server.c -lsdl -lsmain -framework Foundation -framework Cocoa && ./client
   
#include <iostream>
#include <sstream>
#include <string>

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <ApplicationServices/ApplicationServices.h>

#include "Message.h"

class Server
{
  int max_socket_;
  int listen_sock_;
  fd_set read_sockets_;
  
public:

	void PostMouseEvent(CGMouseButton button, CGEventType type, const CGPoint point, int click_count = 1) 
	{
		static int eventNumber = 0;
		CGEventRef theEvent = CGEventCreateMouseEvent(NULL, type, point, button);
		CGEventSetIntegerValueField(theEvent, kCGMouseEventClickState, click_count) ;
		CGEventSetIntegerValueField(theEvent, kCGMouseEventNumber, ++eventNumber); 
		CGEventSetType(theEvent, type);
		CGEventPost(kCGHIDEventTap, theEvent);
		CFRelease(theEvent);
	}

	void left_down() {   
		CGEventRef event = CGEventCreate(NULL);
		CGPoint point = CGEventGetLocation(event); 
		PostMouseEvent(kCGMouseButtonLeft, kCGEventLeftMouseDown, point);
	}

	void left_up() { 
		CGEventRef event = CGEventCreate(NULL);
		CGPoint point = CGEventGetLocation(event); 
		PostMouseEvent(kCGMouseButtonLeft, kCGEventLeftMouseUp, point);
	}

	void left_drag(int x, int y)
	{
		CGEventRef ourEvent = CGEventCreate(NULL);
		CGPoint epoint = CGEventGetLocation(ourEvent);

		CGPoint point;
		point.x = epoint.x + x;
		point.y = epoint.y + y;
		PostMouseEvent(kCGMouseButtonLeft, kCGEventLeftMouseDragged, point);
	}

	void right_down() {
		CGEventRef event = CGEventCreate(NULL);
		CGPoint point = CGEventGetLocation(event); 
		PostMouseEvent(kCGMouseButtonRight, kCGEventRightMouseDown, point);
	}

	void right_up() { 
		CGEventRef event = CGEventCreate(NULL);
		CGPoint point = CGEventGetLocation(event); 
		PostMouseEvent(kCGMouseButtonRight, kCGEventRightMouseUp, point);
	}

	void right_drag(int x, int y)
	{
		CGEventRef ourEvent = CGEventCreate(NULL);
		CGPoint epoint = CGEventGetLocation(ourEvent);

		CGPoint point;
		point.x = epoint.x + x;
		point.y = epoint.y + y;
		PostMouseEvent(kCGMouseButtonRight, kCGEventRightMouseDragged, point);
	}

	void type_key(int key_code)
	{
		CGEventRef e = CGEventCreateKeyboardEvent (NULL, key_code, true);
		CGEventPost(kCGSessionEventTap, e);
		CFRelease(e);
	}

	void move_mouse(int x, int y)
	{
		CGEventRef ourEvent = CGEventCreate(NULL);
		CGPoint epoint = CGEventGetLocation(ourEvent);
		
		CGDisplayModeRef mode = CGDisplayCopyDisplayMode(kCGDirectMainDisplay);
		int width = CGDisplayModeGetWidth(mode);
		int height = CGDisplayModeGetHeight(mode);

		CGPoint point;
		point.x = epoint.x + x;
		point.y = epoint.y + y;
		
		point.x = (point.x < 0) ? 0 : (point.x > width) ? width - 1 : point.x;
		point.y = (point.y < 0) ? 0 : (point.y > height) ? height - 2 : point.y;
		PostMouseEvent(kCGMouseButtonCenter, kCGEventMouseMoved, point);
	}

	void change_flags(int key_code, unsigned int flags)
	{
		CGEventRef e = CGEventCreateKeyboardEvent (NULL, key_code, true);
		CGEventSetFlags(e, (CGEventFlags)flags);
		CGEventPost(kCGSessionEventTap, e);
		CFRelease(e);
	}

	void scroll_wheel(int x, int y)
	{
		CGEventRef ourEvent = CGEventCreate(NULL);
		CGPoint point = CGEventGetLocation(ourEvent);
		
		CGEventRef e = CGEventCreateScrollWheelEvent(NULL, kCGScrollEventUnitPixel, 1, 2);
		
		CGEventSetType(e, kCGEventScrollWheel);
		CGEventSetIntegerValueField(e, kCGScrollWheelEventDeltaAxis1, y);
		CGEventPost(kCGSessionEventTap, e);
		CFRelease(e);
	}

	void left_double_click()
	{
		CGEventRef ourEvent = CGEventCreate(NULL);
		CGPoint point = CGEventGetLocation(ourEvent);
		
		CGEventRef theEvent = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, point, kCGMouseButtonLeft);  
		CGEventSetIntegerValueField(theEvent, kCGMouseEventClickState, 2); 
		CGEventPost(kCGHIDEventTap, theEvent);  
		CGEventSetType(theEvent, kCGEventLeftMouseUp);  
		CGEventPost(kCGHIDEventTap, theEvent);  
		CGEventSetType(theEvent, kCGEventLeftMouseDown);  
		CGEventPost(kCGHIDEventTap, theEvent);  
		CGEventSetType(theEvent, kCGEventLeftMouseUp); 
		CGEventPost(kCGHIDEventTap, theEvent); 
		CFRelease(theEvent); 
	};

	void process_message(const Message& message)
	{
		switch(message.type)
		{
			case LEFT_DOWN:
			{
				std::clog << "left down" << std::endl;
				left_down();
				break;
			}
			
			case LEFT_UP:
			{
				std::clog << "left up" << std::endl;
				left_up();
				break;
			}
			
			case RIGHT_DOWN:
			{
				std::clog << "right down" << std::endl;
				right_down();
				break;
			}
			
			case RIGHT_UP:
			{
				std::clog << "right up" << std::endl;
				right_up();
				break;
			}
			
			case KEY_DOWN:
			{
				std::clog << "key down " << message.key_code << std::endl;    
				type_key(message.key_code);    
				break;
			}
			
			case MOUSE_MOVED:
			{
				std::clog << "mouse moved x: " << message.x << " y: " << message.y << std::endl;
				move_mouse(message.x, message.y);        
				break;
			}
			
			case LEFT_DRAGGED:
			{
				std::clog << "left dragged x: " << message.x << " y: " << message.y << std::endl;
				left_drag(message.x, message.y);        
				break;
			}
			
			case RIGHT_DRAGGED:
			{
				std::clog << "right dragged x: " << message.x << " y: " << message.y << std::endl;
				right_drag(message.x, message.y);
				break;
			}
			
			case FLAGS_CHANGED:
			{
				std::clog << "flags changed " << message.flags << std::endl;
				change_flags(message.key_code, message.flags);
				break;
			}
			
			case SCROLL_WHEEL:
			{
				std::clog << "scroll wheel x: " << message.x << " y: " << message.y << std::endl;
				scroll_wheel(message.x, message.y);        
				break;
			}

			case LEFT_DOUBLE_CLICK:
			{
				std::clog << "left double click" << std::endl;
				left_double_click();
				break;
			}
			
			default:
			{
				std::clog << "unrecognized input received" << std::endl;
			} 
		}
	}

  void start_listening(unsigned int port)
  {  
    listen_sock_ = create_socket(port);
    std::clog << listen_sock_ << std::endl;
    max_socket_ = listen_sock_;
    FD_SET(listen_sock_, &read_sockets_);
    
    listen(listen_sock_, 5);
    std::clog << "waiting for connections" << std::endl;
  };
  
  bool receive() 
  { 
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    
    fd_set working_socks;
    FD_ZERO(&working_socks);
    memcpy(&working_socks, &read_sockets_, sizeof(read_sockets_));

    int readsocks = select(max_socket_ + 1, &working_socks, NULL, NULL, &timeout);

    if (readsocks < 0)
    { 
      std::cerr << "ERROR on accept" << std::endl;
    }

    if (readsocks == 0)
    {
      std::clog << "nothing to process" << std::endl;
    }

    if (readsocks > 0)
    {      
      for (int i = 0; i < max_socket_ + 1; i++)
      {      
        if (FD_ISSET(i, &working_socks))
        {
          if (i == listen_sock_)
          {
            std::clog << "new incoming connection" << std::endl;

            int incoming_socket = accept(listen_sock_, NULL, NULL);

            if (incoming_socket < 0)
            {
              std::cerr << "ERROR accepting incoming connection" << std::endl;
            }

            FD_SET(incoming_socket, &read_sockets_);
            max_socket_ = incoming_socket;
          }
          else
          {  
            std::clog << "data transfer" << std::endl;
      
            char buffer[256];
            bzero(buffer, 256);
          
            int result = read(i, buffer, sizeof(buffer));
      
            if (result < 0)
            { 
              std::cerr << "ERROR reading from socket" << std::endl;
            }
            
            if (result == 0)
            {
              std::clog << "no data" << std::endl;
              std::clog << "closing connection" << std::endl;
              
              shutdown (i, 2);
              FD_CLR(i, &read_sockets_);
            }
          
            if (result > 0)
            {
              Message message;
              memcpy(&message, &buffer, sizeof(Message));    
              process_message(message);
            }
          }
        }
      }
    }
    
    return true;
  };

  Message data() { return Message(); };
  
private:
  
  void set_non_blocking(int sock)
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
  }
  
  int create_socket(unsigned int port)
  {
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
    {
      std::cerr << "ERROR opening socket" << std::endl;
    }
    
    set_non_blocking(sock);  
    
    if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
      std::cerr << "ERROR on binding" << std::endl;
    }
    
    return sock;    
  }
    
};

/*int main(int argc, char *argv[])
{
  if (argc < 2) 
  {
    std::cerr << "ERROR, no port provided" << std::endl;
    exit(1);
  }
  
  Server server;
  server.start_listening(atoi(argv[1]));
  
  while(server.receive());
	
	return 0;
}*/
