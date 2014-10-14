#pragma once

#include <vector>
#include <atomic>

#include "choco/error/errno.h"

#include "conn.h"

namespace choco{
namespace session{

	class pool{
	public:
		typedef std::vector<conn*>::iterator iterator;

	public:
		pool();
		virtual ~pool();

		error initialize();
		void cleanup();

		iterator begin();
		iterator end();

		int get_size();
		int get_accepted();

	private:
		std::vector<conn*> conns;

		int size;
		std::atomic<int> accepted;
	};
};};