#pragma once

#include "choco/error/errno.h"

namespace choco{
namespace log{
	
	enum log_filter{
		filter_output =1,
		filter_info =2,
		filter_warn = 4,
		filter_error = 8,

		filter_system = 1024,
	};

	error initialize();
	void cleanup();

	void set_log_filter(
		int filter);
	int get_log_filter();

	void system(
		char *fmt, ...);

	void output(
		char *fmt, ...);
	void info(
		char *fmt, ...);
	void warn(
		char *fmt, ...);
	void error(
		char *fmt, ...);
};};