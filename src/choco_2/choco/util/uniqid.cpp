#include "uniqid.h"

#include <time.h>
#include <Windows.h>

#include <stdio.h>

/* singe 1970:00:00:00 */
#define epoch ((unsigned __int64) 116444736000000000ULL)

namespace choco{
namespace util{

	unsigned long long uniqid(){
		FILETIME file;
		SYSTEMTIME sys;
		ULARGE_INTEGER ularge;

		GetSystemTime(&sys);
		SystemTimeToFileTime(&sys, &file);

		ularge.LowPart = file.dwLowDateTime;
		ularge.HighPart = file.dwHighDateTime;

		int sec = 
			 (long)((ularge.QuadPart - epoch) / 10000000L);
		int usec =
			(long)(sys.wMilliseconds * 1000) % 0x100000;

		
		unsigned long long id;

		memset(((char*)&id)+4, sec, sizeof(int));
		memset(((char*)&id), usec, sizeof(int));
		
		return id;
	}
};};