//gcc -o server server.c -lsdl -lsmain -framework Foundation -framework Cocoa && ./client
   
#include <SDL/SDL.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <ApplicationServices/ApplicationServices.h>

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

void error(char *msg)
{
  perror(msg);
  exit(1);
}

void PostMouseEvent(CGMouseButton button, CGEventType type, const CGPoint point) 
{
 CGEventRef theEvent = CGEventCreateMouseEvent(NULL, type, point, button);
 CGEventSetType(theEvent, type);
 CGEventPost(kCGHIDEventTap, theEvent);
 CFRelease(theEvent);
}

void click() { 
  CGEventRef event = CGEventCreate(NULL);
  CGPoint point = CGEventGetLocation(event); 
  PostMouseEvent(kCGMouseButtonLeft, kCGEventLeftMouseDown, point);
  PostMouseEvent(kCGMouseButtonLeft, kCGEventLeftMouseUp, point);
}

int main(int argc, char *argv[])
{
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_SetVideoMode(640, 480, 0, SDL_HWPALETTE);
    
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
      error("ERROR opening socket");
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    
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
      error("ERROR on accept");
    }
    
    while(1)
    {
      SDL_Event event;
      SDL_PollEvent(&event);
    
      bzero(buffer,256);
      n = read(newsockfd,buffer,255);
      if (n < 0)
      { 
        error("ERROR reading from socket");
      }
      
      std::stringstream input;
      input << buffer;
      
      if (input.str() == "CLICK")
      {
        click();
      }
      else if(input.str().find("KEY_") != std::string::npos)
      {
        std::vector<std::string> coords = split(input.str().c_str(), '_');
        std::clog << coords.front().c_str() << std::endl;
      }
      else
      {
        SDL_ShowCursor(true);      
        SDL_Cursor* cursor = SDL_GetCursor();
        std::vector<std::string> coords = split(input.str().c_str(), '_');
      
        CGEventRef ourEvent = CGEventCreate(NULL);
        CGPoint epoint = CGEventGetLocation(ourEvent);
      
        CGPoint point;
        point.x = epoint.x + atoi(coords.front().c_str());
        point.y = epoint.y + atoi(coords.back().c_str());
        CGWarpMouseCursorPosition(point);
      }
    }
  
    return 0; 
}
