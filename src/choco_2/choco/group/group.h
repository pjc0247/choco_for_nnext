#pragma once

#include <concurrent_vector.h>

#include "choco/intf/sendable.h"

#include <concurrent_unordered_set.h>

namespace choco{
namespace session{
	class conn;
};

	class group : intf::sendable{
	public:
		group();
		virtual ~group();

		void add_child(
			group *c);
		void remove_child(
			group *c);

		virtual int send(
			void *data,int len);

	private:
		concurrency::concurrent_vector<session::conn*> conns;
		concurrency::concurrent_vector<group*> children;
	};
};