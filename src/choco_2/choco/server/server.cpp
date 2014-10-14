#include "server.h"

#include "choco/config/config.h"
#include "choco/config/keys.h"
#include "choco/log/log.h"

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
		int pool_size, port, backlog_size;
		string host;

		ret = config::get_as_int(
			config::keys::server_pool_size, pool_size);
		_RETURN_ERR(ret);

		ret = config::get_as_int(
			config::keys::server_backlog_size, backlog_size);
		_RETURN_ERR(ret);

		ret = config::get_as_string(
			config::keys::server_host, host);
		_RETURN_ERR(ret);

		ret = config::get_as_int(
			config::keys::server_port, port);
		_RETURN_ERR(ret);


		hostent* hostInfo =
			gethostbyname( host.c_str() );
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

		/* init worker_pool */
		worker_pool = new worker::pool();
		worker_pool->initialize(this);

		/* init session_pool */
		session_pool = new session::pool();
		session_pool->initialize();
		for(auto conn : *session_pool)
			conn->accept(sock);

		log::system(
			"server - initialized / addr : %s:%d, backlog : %d\n",
			host.c_str(), port, backlog_size);

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