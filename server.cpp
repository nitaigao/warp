//gcc -o server server.c -lsdl -lsmain -framework Foundation -framework Cocoa && ./client
   
#include <iostream>
#include <sstream>
#include <string>

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <ApplicationServices/ApplicationServices.h>

enum MessageTypes
{
	LEFT_DOWN = 0,
	RIGHT_DOWN = 1,
	LEFT_UP = 2,
	RIGHT_UP = 3,
	KEY_DOWN = 4,
	MOUSE_MOVED = 5,
	LEFT_DRAGGED = 6,
	RIGHT_DRAGGED = 7,
	FLAGS_CHANGED = 8,
	SCROLL_WHEEL = 9
};

struct Message 
{
	int type;
	int x;
	int y;
	int key_code;
	unsigned int flags;
};

void PostMouseEvent(CGMouseButton button, CGEventType type, const CGPoint point) 
{
  static int eventNumber = 0;
  CGEventRef theEvent = CGEventCreateMouseEvent(NULL, type, point, button);
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
    
    default:
    {
      std::clog << "unrecognized input received" << std::endl;
    } 
  }
}

int main(int argc, char *argv[])
{
  int sockfd, newsockfd, portno;
  char buffer[256];
  
  struct sockaddr_in serv_addr, cli_addr;
  
  int n;

  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0)
  {
    std::cerr << "ERROR opening socket" << std::endl;
  }
  
  bzero((char*)&serv_addr, sizeof(serv_addr));
  
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
  {
    //error("ERROR on binding");
  }
  
  listen(sockfd,5);
  socklen_t socklen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,&socklen);
  
  if (newsockfd < 0)
  { 
    std::cerr << "ERROR on accept" << std::endl;
  }
  
  while(1)
  {
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0)
    { 
      std::cerr << "ERROR reading from socket" << std::endl;
    }
    
    Message message;
    memcpy(&message, &buffer, sizeof(Message));
    
    process_message(message);
  }

  return 0; 
}