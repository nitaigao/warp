#ifndef ISERVERCOMMAND_H
#define ISERVERCOMMAND_H

	#include "Message.h"

	class IExitCommand
	{
		
	public:
		
		virtual void Execute(const Message& message) = 0;
		
	};

#endif