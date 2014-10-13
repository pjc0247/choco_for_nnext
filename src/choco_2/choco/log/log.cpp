#include "log.h"

#include <stdio.h>
#include <stdarg.h>

namespace choco{
namespace log{

	const int BUFFER_MAX = 512;

	int filter = ~0;

	int initialize(){
		filter = ~0;

		return 0;
	}
	void cleanup(){
	}

	void set_log_filter(
		int _filter){

		filter = _filter;
	}
	int get_log_filter(){
		return filter;
	}

	void system(
		char *fmt, ...){

		if(!(filter & filter_system))
			return;

		char buffer[BUFFER_MAX];
		va_list va;
		va_start(va, fmt);
			vsprintf(buffer, fmt, va);
			printf(buffer);
		va_end(va);
	}
	void output(
		char *fmt, ...){

		if(!(filter & filter_output))
			return;

		char buffer[BUFFER_MAX];
		va_list va;
		va_start(va, fmt);
			vsprintf(buffer, fmt, va);
			printf(buffer);
		va_end(va);
	}
	void info(
		char *fmt, ...){

		if(!(filter & filter_info))
			return;

		char buffer[BUFFER_MAX];
		va_list va;
		va_start(va, fmt);
			vsprintf(buffer, fmt, va);
			printf(buffer);
		va_end(va);
	}
	void warn(
		char *fmt, ...){

		if(!(filter & filter_warn))
			return;

		char buffer[BUFFER_MAX];
		va_list va;
		va_start(va, fmt);
			vsprintf(buffer, fmt, va);
			printf(buffer);
		va_end(va);
	}
	void error(
		char *fmt, ...){

		if(!(filter & filter_error))
			return;

		char buffer[BUFFER_MAX];
		va_list va;
		va_start(va, fmt);
			vsprintf(buffer, fmt, va);
			printf(buffer);
		va_end(va);
	}
};};