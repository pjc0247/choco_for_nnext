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

		void join(
			intf::sendable *s);
		void remove(
			intf::sendable *s);

		virtual int send(
			void *data,int len);

	private:
		concurrency::concurrent_vector<intf::sendable*> children;
	};
};