#pragma once

namespace choco{
namespace server{
	class server;
};

namespace mt{

	void sleep_async(
		server::server *s,
		int ms);
};};