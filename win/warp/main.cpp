
#include <iostream>

#include "../../warp/Constants.hpp"
#include "../../warp/Server.h"

void pump_messages()
{

}

int main()
{
	Server server;
	server.start_listening(SERVER_PORT);

	while(true)
	{
		pump_messages();

		server.receive();
	};

	return 0;
}