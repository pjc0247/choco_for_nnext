#pragma once

#include <vector>
#include <thread>

#include "choco/error/errno.h"

namespace choco{
namespace server{

	class server;

namespace worker{

	class pool{
	public:
		pool();
		virtual ~pool();

		error initialize(
			server *server);
		void cleanup();

	private:
		std::vector<std::thread> workers;
	};
};};};