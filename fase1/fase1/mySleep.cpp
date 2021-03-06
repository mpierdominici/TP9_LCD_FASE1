#include "mySleep.h"

#ifdef unix
	#include <unistd.h>
#elif  defined _WIN32 || defined _WIN64
	#include <windows.h>
#endif

void mySleep(int sleepMs)
{
#ifdef unix
    usleep(sleepMs * 1000);   // usleep takes sleep time in us (1 millionth of a second)
#elif defined _WIN32 || defined _WIN64
    Sleep(sleepMs);
#endif

}