//g++ -o client client.cpp -lsdl -lsmain -framework Foundation -framework Cocoa -framework Foundation && ./client

#include <SDL/SDL.h>
#include <iostream>
#include <sstream>

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

int soc = 0;
bool connected = false;

bool connect_server(const std::string& host)
{
  int portno, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[256];

  soc = socket(AF_INET, SOCK_STREAM, 0);

  if (soc < 0) 
  {
    std::clog << "ERROR, no such host" << std::endl;
  } 

  server = gethostbyname(host.c_str());

  if (server == NULL)
  {
    std::clog << "ERROR, no such host" << std::endl;
    exit(0);
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;

  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

  serv_addr.sin_port = htons(12345);

  if (connect(soc, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
  { 
    std::clog << "ERROR connecting" << std::endl;
  }
  
  return true;
}

void send_message(const std::string& message)
{
  int n = write(soc, message.c_str(), strlen(message.c_str()));

  if (n < 0)
  {
   std::clog << "ERROR writing to socket" << std::endl;
  }
}

void send_server(int x, int y)
{
  std::stringstream message;
  message << x << "_" << y;
  send_message(message.str());
}

void send_server_click()
{
  send_message("CLICK");
}

void send_server_key(const std::string& key)
{
  std::stringstream code;
  code << "KEY_" << key;
  //send_message(code.str().c_str());
}

OSStatus mouse_up(EventHandlerCallRef inCaller, EventRef inEvent, void *pUserData)
{
  send_server_click();
}

EventHandlerRef         m_ApplicationMouseDragEventHandlerRef;                  
EventHandlerRef         m_MonitorMouseUpEventHandlerRef;

void register_events()
{
  OSStatus ErrStatus;

  static const EventTypeSpec kMouseUpEvents[] =
  {
    { kEventClassMouse, kEventMouseUp }
  };

  ErrStatus = InstallEventHandler(GetEventMonitorTarget(), NewEventHandlerUPP(mouse_up), GetEventTypeCount(kMouseUpEvents), kMouseUpEvents, NULL, &m_MonitorMouseUpEventHandlerRef);
  ErrStatus = InstallApplicationEventHandler(NewEventHandlerUPP(mouse_up), GetEventTypeCount(kMouseUpEvents), kMouseUpEvents, NULL, &m_ApplicationMouseDragEventHandlerRef);
}

char getUnicodeValue( const SDL_KeyboardEvent &key )
{
    // magic numbers courtesy of SDL docs :)
    const int INTERNATIONAL_MASK = 0xFF80, UNICODE_MASK = 0x7F;

    int uni = key.keysym.unicode;

    if( uni == 0 ) // not translatable key (like up or down arrows)
    {
        // probably not useful as string input
        // we could optionally use this to get some value
        // for it: SDL_GetKeyName( key );
        return 0;
    }

    if( ( uni & INTERNATIONAL_MASK ) == 0 )
    {
        if( SDL_GetModState() & KMOD_SHIFT )
        {
            return (char)(toupper(uni & UNICODE_MASK));
        }
        else
        {
            return (char)(uni & UNICODE_MASK);
        }
    }
    else // we have a funky international character. one we can't read :(
    {
        // we could do nothing, or we can just show some sign of input, like so:
        return '?';
    }
}


int main( int argc, char* args[] )
{
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_SetVideoMode(9999, 9999, 0, SDL_HWPALETTE);
  
  
  CGRect mainMonitor = CGDisplayBounds(CGMainDisplayID());
  CGFloat monitorHeight = CGRectGetHeight(mainMonitor);
  CGFloat monitorWidth = CGRectGetWidth(mainMonitor);
  CGPoint center_screen;
  center_screen.x = monitorWidth / 2;
  center_screen.y = monitorHeight / 2;
  
  CGWarpMouseCursorPosition(center_screen);
  CGAssociateMouseAndMouseCursorPosition(false);
  //CGDisplayHideCursor(kCGDirectMainDisplay);
  
  register_events();
  
  bool quit = false;

  bool connected = connect_server(args[1]);
  SDL_Event event;
  
  while(quit == false)
  {    
    if(connected)
    {    
      if (SDL_PollEvent(&event))
      {
        switch(event.type)
        {
          case SDL_MOUSEBUTTONUP:
          {
            send_server_click();  
          }
          case SDL_KEYUP:
          {
            std::stringstream input;
            input << getUnicodeValue(event.key);
            send_server_key(input.str());
          }
        }
      }
      
      CGMouseDelta dx, dy; 
      CGGetLastMouseDelta(&dx, &dy);

      CGEventRef ourEvent = CGEventCreate(NULL);
      CGPoint position = CGEventGetLocation(ourEvent);

      int x = dx;
      int y = dy;

      if (x != 0 || y != 0)
      {
        send_server(x, y);
        CGWarpMouseCursorPosition(center_screen);
      } 
    }
  }
}
