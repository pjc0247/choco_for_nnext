#include "choco.h"

#include <WinSock2.h>
#include <Windows.h>

#include "choco.h"

#define INIT_SUBSYSTEM(init_flag, init_path) \
	if(subsystems & init_flag){ \
		int ret = init_path::initialize(); \
		if(ret){ \
			log::error( \
			#init_path " - failed to init / err : %d\n", \
				ret); \
			return ret; \
		} \
		else{ \
			initialized_subsystems |= \
				init_flag; \
		} \
	}

namespace choco{

	int initialized_subsystems;

	error initialize(
		int subsystems){

		WSAData wsa;
		if(WSAStartup(MAKEWORD(2, 2), &wsa))
			return false;

		log::system(
			"choco - initialized\n");

		initialized_subsystems = 0;
		INIT_SUBSYSTEM(init_log, log);
		INIT_SUBSYSTEM(init_parallel, parallel);
		INIT_SUBSYSTEM(init_mysql, orm);

		return errorno::none;
	}
	void cleanup(){
		WSACleanup();
	}
};