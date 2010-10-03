#include "Constants.hpp"
#include "Message.h"
#include "Exit.h"

#include <vector>

int main()
{
  Exit exit;
  exit.start_listening(SERVER_PORT);
  
	while(true)
	{
		exit.receive();
	};

	return 0;
}