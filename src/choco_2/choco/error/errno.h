#pragma once

#include "error.h"

namespace choco{
namespace errorno{

	enum standard_error : error{
		none =0,
		no_memory,
		invalid_param,
		system_error,
		file_not_found
	};
	
	enum config_error : error{
		parse_error =101,
		key_not_exists
	};

	enum server_error : error{
		socket_error = 201,
		host_error,
		bind_error,
		listen_error,
		iocp_error
	};

	enum mysql_error : error{
		connection_failed = 401
	};
};};