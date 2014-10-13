#pragma once

#include "choco/error/error.h"
#include "choco/intf/raw.h"
#include "choco/session/pool.h"

#include "worker/worker.h"
#include "worker/pool.h"

namespace choco{
namespace server{

	enum event_type{
		event_accept=1,
		event_recv,
		event_task_complete
	};

	class server{
		friend void worker::func(server*);

	public:
		server();
		server(
			intf::raw *i);
		virtual ~server();

		error open();
		void close();


		error notify_event(
			int type,
			void *data);
		error notify_task_complete(
			mt::microthread *mt,
			void *data);

		socket get_socket();
		intf::raw *get_interface();
		void *get_iocp_handle();

	private:
		session::pool *session_pool;
		worker::pool *worker_pool;

		intf::raw *intf;

		socket sock;
		sockaddr_in local_addr;
		void *iocp;
	};
};};