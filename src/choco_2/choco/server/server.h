#pragma once

#include <vector>

#include "choco/error/error.h"
#include "choco/intf/raw.h"
#include "choco/session/pool.h"

#include "worker/worker.h"
#include "worker/pool.h"

namespace choco{
namespace intf{
	class hook;
};

namespace server{

	enum event_type{
		event_accept=1,
		event_recv,
		event_task_complete
	};
	enum hook_type{
		hook_recv=1,
		hook_send
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

		void add_hooker(
			intf::hook *h);

		socket get_socket();
		intf::raw *get_interface();
		void *get_iocp_handle();

		error process_connection(
			session::conn *client);
		error process_disconnection(
			session::conn *client);
		error process_recv(
			session::conn *client, event *evt,
			void *buffer, int len);
		error process_send(
			session::conn *client, event *evt,
			void *buffer, int len);
		error process_recv_completion(
			session::conn *client, event *evt,
			int len);

	private:
		error process_hook(
			hook_type hook,
			session::conn *client,
			void *in_data, int in_len,
			void *&out_data, int &out_len);

	private:
		session::pool *session_pool;
		worker::pool *worker_pool;

		intf::raw *intf;

		socket sock;
		sockaddr_in local_addr;
		void *iocp;

		std::vector<intf::hook*> hookers;
	};
};};