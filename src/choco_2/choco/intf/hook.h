#pragma once

#include "choco/error/errno.h"

namespace choco{
namespace session{
	class conn;
};

namespace intf{

	class hook{
	public:

		virtual error on_send(
			session::conn *client,
			void *in_data, int in_len,
			void *&out_data, int &out_len) =0;
		virtual error on_recv(
			session::conn *client,
			void *in_data, int in_len,
			void *&out_data, int &out_len) =0;
	};
};};