#pragma once

#include "choco/error/errno.h"

#include "choco/common.h"
#include "choco/macro.h"

#include "choco/config/config.h"
#include "choco/config/keys.h"

#include "choco/log/log.h"

#include "choco/mem/mem.h"

#include "choco/mt/mt.h"
#include "choco/mt/microthread.h"
#include "choco/mt/async.h"
#include "choco/mt/sleep.h"

#include "choco/intf/raw.h"
#include "choco/intf/handler.h"
#include "choco/intf/hook.h"

#include "choco/packet/packet.h"

#include "choco/session/conn.h"
#include "choco/session/pool.h"

#include "choco/server/server.h"

#include "choco/http/client.h"
#include "choco/http/open_uri.h"

#include "choco/parallel/async.h"
#include "choco/parallel/parallel.h"
#include "choco/parallel/schedule.h"

#include "choco/orm/orm.h"

//#include "choco/debug/invoke.h"

#include "choco/util/uniqid.h"

namespace choco{

	enum subsystem{
		init_log =1,
		init_parallel =2,
		init_mysql =4,
		init_mem =8
	};

	error initialize(
		int subsystems =0);
	void cleanup();
};