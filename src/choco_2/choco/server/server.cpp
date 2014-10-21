#include "server.h"

#include <MSWSock.h>

#include "choco/config/config.h"
#include "choco/config/keys.h"
#include "choco/log/log.h"
#include "choco/intf/hook.h"

using namespace std;

namespace choco{
namespace server{

	server::server(){
	}
	server::server(intf::raw *i) :
		intf(i){
	}
	server::~server(){
	}

	error server::open(){
		int ret;
		int pool_size;
		int backlog_size, port;
		string host;

		_CONFIG_INT(server_pool_size, pool_size);
		_CONFIG_INT(server_backlog_size, backlog_size);
		_CONFIG_INT(server_port, port);
		_CONFIG_STR(server_host, host);

		ret = initialize_socket(
			host, port,
			backlog_size);
		_RETURN_ERR(ret);

		ret = initialize_iocp(
			pool_size);
		_RETURN_ERR(ret);

		/* init worker_pool */
		worker_pool = new worker::pool();
		worker_pool->initialize(this);

		/* init session_pool */
		session_pool = new session::pool();
		session_pool->initialize(this);
		for(auto conn : *session_pool)
			conn->accept(sock);

		log::system(
			"server - initialized / addr : %s:%d, backlog : %d\n",
			host.c_str(), port, backlog_size);

		return errorno::none;
	}
	error server::initialize_socket(
		const std::string &host, int port,
		int backlog_size){

		int ret;

		hostent* hostInfo =
			gethostbyname(host.c_str());
		if(hostInfo == nullptr)
			return errorno::host_error;

		sock = WSASocket(
			AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		if(sock == INVALID_SOCKET)
			return errorno::socket_error;

		local_addr.sin_family = AF_INET;
		local_addr.sin_addr.s_addr = 
			*(unsigned long*)hostInfo->h_addr_list[0];
		local_addr.sin_port = htons( port );
		ret = ::bind(
			sock, (SOCKADDR*)&local_addr, sizeof(sockaddr_in));
		if(ret == SOCKET_ERROR)
			return errorno::bind_error;

		ret = listen(sock, backlog_size);
		if(ret == SOCKET_ERROR)
			return errorno::listen_error;

		return errorno::none;
	}
	error server::initialize_iocp(
		int pool_size){

		iocp = CreateIoCompletionPort(
			INVALID_HANDLE_VALUE, nullptr,
			0, pool_size);
		if(iocp == INVALID_HANDLE_VALUE)
			return errorno::iocp_error;

		HANDLE h_ret;
		h_ret = CreateIoCompletionPort(
			(HANDLE)sock, iocp,
			(ULONG_PTR)0, 0);
		if( h_ret == INVALID_HANDLE_VALUE )
			return errorno::iocp_error;

		return errorno::none;
	}

	error server::notify_event(
		int type,
		void *data){

		static event evt;

		memset(&evt, 0, sizeof(event));
		evt.data = data;
		evt.type = type;

		PostQueuedCompletionStatus(
			iocp, 0,
			0, &evt); 

		return errorno::none;
	}
	error server::notify_task_complete(
		mt::microthread *mt,
		void *data){

		static event evt;

		memset(&evt, 0, sizeof(event));
		evt.data = data;
		evt.mt = mt;
		evt.type = event_task_complete;

		PostQueuedCompletionStatus(
			iocp, 0,
			0, &evt);
		
		return errorno::none;
	}

	void server::add_hooker(
		intf::hook *h){

		hookers.push_back(h);
	}

	error server::process_connection(
		session::conn *client){

		HANDLE h_ret;
		int localSize, removeSize;
		socket server_sock = sock;
		socket conn_sock = client->get_socket();

		GetAcceptExSockaddrs(
			client->get_buffer(), 0,
			sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16,
			(sockaddr **)&client->get_local_addr(), &localSize,
			(sockaddr **)&client->get_remote_addr(), &removeSize);

		setsockopt(
			conn_sock,
			SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
			(char *)&server_sock, sizeof(socket));

		h_ret = CreateIoCompletionPort(
			(HANDLE)conn_sock,
			iocp, 0,0);
		if(h_ret == INVALID_HANDLE_VALUE)
			return errorno::iocp_error;

		intf->on_connected(client);

		return errorno::none;
	}
	error server::process_disconnection(
		session::conn *client){

		intf->on_disconnected(client);

		return errorno::none;
	}
	error server::process_recv(
		session::conn *client, event *evt,
		void *buffer, int len){

		DWORD flags = 0;
		DWORD received;
		WSABUF buf;
		buf.buf = (char*)buffer;
		buf.len = len;

		memset(evt, 0, sizeof(event));
		evt->conn = client;
		evt->type = event_recv;

		int ret = WSARecv(
			client->get_socket(),
			&buf, 1,
			&received, &flags,
			evt, nullptr);

		return errorno::none;
	}
	error server::process_send(
		session::conn *client, event *evt,
		void *buffer, int len){

		void *out_buffer = buffer;
		int out_len = len;
		process_hook(
			hook_send, client,
			buffer, len,
			out_buffer, out_len);

		DWORD sent;
		WSABUF buf;
		buf.buf = (char*)out_buffer;
		buf.len = out_len;

		WSASend(
			client->get_socket(),
			&buf, 1, &sent,
			0, nullptr, nullptr);

		if(out_buffer != nullptr)
			free(out_buffer);

		return errorno::none;
	}
	error server::process_recv_completion(
		session::conn *client, event *evt,
		int len){

		void *in_buffer =
			client->get_buffer() + client->get_buffer_ptr();
		void *out_buffer = in_buffer;
		int out_len = len;

		client->incr_buffer_ptr(len);
		process_hook(
			hook_recv, client,
			in_buffer, len,
			out_buffer, out_len);
		intf->on_data(
			client,
			out_buffer, out_len);

		if(out_buffer != nullptr)
			free(out_buffer);

		return errorno::none;
	}
	error server::process_hook(
		hook_type hook,
		session::conn *client,
		void *in_data, int in_len,
		void *&out_data, int &out_len){

		void *orig_in_data = in_data;

		for(intf::hook* h : hookers){
			switch(hook){
			case hook_recv:
				h->on_recv(client,
					in_data, in_len, out_data, out_len);
				break;
			case hook_send:
				h->on_send(client,
					in_data, in_len, out_data, out_len);
				break;
			}

			/* cleanup */
			if(in_data != orig_in_data)
				free(in_data);
		
			/* update for next hook */
			in_data = out_data;
			in_len = out_len;
		}

		return errorno::none;
	}

	socket server::get_socket(){
		return sock;
	}
	intf::raw *server::get_interface(){
		return intf;
	}
	void *server::get_iocp_handle(){
		return iocp;
	}
};};