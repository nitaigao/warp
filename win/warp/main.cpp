
#include <iostream>
#include <WinSock.h>
#include <vector>

#include "../../warp/Constants.hpp"
#include "../../warp/Message.h"
#include "../../warp/Exit.h"

#include <vector>

int main()
{
  Exit exit;
  exit.start_listening(server_port);
  
	while(true)
	{
    exit.receive();
	};

	return 0;
}