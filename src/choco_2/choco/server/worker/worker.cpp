#include "worker.h"

#include "choco/common.h"
#include "choco/log/log.h"
#include "choco/error/errno.h"
#include "choco/server/server.h"
#include "choco/mt/mt.h"
#include "choco/mt/microthread.h"

#include <MSWSock.h>

namespace choco{
namespace server{
namespace worker{

	error process_accept(
		server *s, event *evt){

		int ret;
		session::conn *conn;

		conn = evt->conn;
		ret = true; /* ÀÏ´Ü ¹«Á¶°Ç ½Â³« */
		
		/* ¿¬°á ½Â³« */
		if(ret != 0){
			HANDLE h_ret;
			int localSize, removeSize;
			socket server_sock = s->get_socket();
			socket conn_sock = conn->get_socket();

			GetAcceptExSockaddrs(
				conn->get_buffer(), 0,
				sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16,
				(sockaddr **)&conn->get_local_addr(), &localSize,
				(sockaddr **)&conn->get_remote_addr(), &removeSize);

			setsockopt(
				conn_sock,
				SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
				(char *)&server_sock, sizeof(socket));

			h_ret = CreateIoCompletionPort(
				(HANDLE)conn_sock,
				s->get_iocp_handle(), 0,0);
			if( h_ret == INVALID_HANDLE_VALUE )
				return errorno::iocp_error;
		}
		/* ¿¬°á °ÅºÎ, ¼ÒÄÏ ÀçÈ°¿ë */
		else{
			evt->conn->accept(
				s->get_socket());
		}

		s->get_interface()->on_connected(conn);
		
		return errorno::none;
	}
	error process_recv(
		server *s, event *evt,
		DWORD tranferred){
		
		session::conn *conn;
		conn = evt->conn;

		conn->incr_buffer_ptr(tranferred);
		s->get_interface()->on_data(
			conn,
			conn->get_buffer(), tranferred);

		return errorno::none;
	}
	error processs_task_complete(
		server *s, event *evt){

		evt->mt->schedule();

		return errorno::none;
	}

	void func(
		server *s){

		mt::initialize();
		while(true){
			BOOL ret;
			DWORD tranferred;
			DWORD key;
			event *evt;

			ret = GetQueuedCompletionStatus(
				s->iocp,
				&tranferred,
				(PULONG_PTR)&key, (OVERLAPPED**)&evt,
				INFINITE);

			if(!ret)
				continue;

			switch(evt->type){
			case event_accept:{
				int ret = 
					process_accept(s, evt);
				if(ret != errorno::none){
					log::error(
						"server::worker - failed to accept / socket : %d, errorno : %d\n",
						s->get_socket(), ret);
				}} break;
			case event_recv:
				process_recv(s, evt, tranferred);
				break;
			case event_task_complete:
				processs_task_complete(s, evt);
				break;
			}
		}

		mt::cleanup();
	}
};};};