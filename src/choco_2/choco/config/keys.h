#pragma once

#include <string>

#define _DEF_KEY(section,key) \
	const std::string section##_##key = \
		#section "::" #key;

namespace choco{
namespace config{
namespace keys{

	/* log */
	_DEF_KEY(log, filter);

	/* server */
	_DEF_KEY(server, host);
	_DEF_KEY(server, port);
	_DEF_KEY(server, backlog_size);
	_DEF_KEY(server, pool_size);

	/* session */
	_DEF_KEY(session, pool_size);
	_DEF_KEY(session, inbuf_size);

	/* parallel (thread_pool) */
	_DEF_KEY(parallel, scheduler_interval);
	_DEF_KEY(parallel, pool_size);

	/* mysql */
	_DEF_KEY(mysql, host);
	_DEF_KEY(mysql, port);
	_DEF_KEY(mysql, user);
	_DEF_KEY(mysql, password);
	_DEF_KEY(mysql, db_name);
	_DEF_KEY(mysql, pool_size);
};};};