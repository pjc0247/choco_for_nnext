#pragma once

#include <vector>

#include "choco/error/errno.h"

#include "conn.h"

namespace choco{
namespace session{

	class pool{
	public:
		static const int POOL_MAX = 1024;

		typedef std::vector<conn*>::iterator iterator;

	public:
		pool();
		virtual ~pool();

		error initialize();
		void cleanup();

		iterator begin();
		iterator end();

	private:
		std::vector<conn*> conns;

		int size;
	};
};};