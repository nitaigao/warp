#ifndef TIME_H_
#define TIME_H_

	#include <stdlib.h>
	#include <sys/time.h>

class Time
{
	
public:

	static unsigned int get()
	{
		struct timeval tv;
		struct timezone tz;
		struct tm *tm;
		gettimeofday(&tv, &tz);
		tm = localtime(&tv.tv_sec);
		return ((tm->tm_sec * 1000) + (tv.tv_usec / 1000));	
	}
	
};

#endif