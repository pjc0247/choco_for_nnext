#include "conn.h"

#include <MSWSock.h>
#include <WinSock2.h>
#include <Windows.h>

#include "choco/server/server.h"

using namespace std;

namespace choco{
namespace session{

	conn::conn() :
		sock(0),
		inbuf(nullptr), inbuf_ptr(0), inbuf_max(0),
		state(0){
	}
	conn::~conn(){
	}

	error conn::open(
		int inbuf_size){

		sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(sock == INVALID_SOCKET)
			return errorno::socket_error;
		
		inbuf = (char*)malloc(inbuf_size);
		if(inbuf == nullptr)
			return errorno::no_memory;

		memset(&evt,0, sizeof(event));
		evt.conn = this;

		inbuf_max = inbuf_size;
		
		return errorno::none;
	}
	error conn::accept(
		socket server_sock){

		BOOL ret;

		TransmitFile(
			sock, nullptr, 0, 0,
			(OVERLAPPED*)&evt, nullptr,
			TF_DISCONNECT | TF_REUSE_SOCKET);		

		memset(&evt, 0, sizeof(event));
		evt.conn = this;
		evt.type = server::event_accept;
		ret = AcceptEx(
			server_sock, sock,
			inbuf, 0,
			sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
			nullptr, (OVERLAPPED*)&evt);

		return errorno::none;
	}
	void conn::close(){
		if(sock != 0){
			LINGER linger_opt;
			linger_opt.l_onoff = 1;
			linger_opt.l_linger = 0;

			set_opt(
				SOL_SOCKET, SO_LINGER,
				(char*)&linger_opt, sizeof(LINGER));

			closesocket(sock);
		}

		sock = 0;
	}

	int conn::recv(
		int len){

		DWORD flags = 0;
		DWORD received;
		WSABUF buf;
		buf.buf = inbuf;
		buf.len = len;
		inbuf_ptr = 0;

		memset(&evt, 0, sizeof(event));
		evt.conn = this;
		evt.type = server::event_recv;

		int r = WSARecv(
			sock,
			&buf, 1,
			&received, &flags,
			&evt, nullptr);

		return true;
	}
	int conn::recv_append(
		int len){

		if(inbuf_max <= inbuf_ptr+len)
			return false;

		DWORD flags = 0;
		DWORD received;
		WSABUF buf;
		buf.buf = inbuf + inbuf_ptr;
		buf.len = len;

		memset(&evt, 0, sizeof(event));
		evt.conn = this;
		evt.type = server::event_recv;

		WSARecv(
			sock,
			&buf, 1,
			&received, &flags,
			&evt, nullptr);
			
		return true;
	}
	int conn::send(
		void *data,int len){

		DWORD sent;
		WSABUF buf;
		buf.buf = (char*)data;
		buf.len = len;

		WSASend(
			sock,
			&buf, 1, &sent,
			0,
			nullptr, nullptr);

		return true;
	}

	int conn::set_opt(
		int level, int optname,
		const char *optval,int optlen){

		return setsockopt(
			sock,
			level, optname,
			optval,optlen);
	}
	int conn::get_opt(
		int level, int optname,
		char *optval,int *optlen){

		return getsockopt(
			sock,
			level, optname,
			optval,optlen);
	}

	char *conn::get_buffer(){
		return inbuf;
	}
	int conn::get_buffer_ptr(){
		return inbuf_ptr;
	}
	socket conn::get_socket(){
		return sock;
	}
	sockaddr_in &conn::get_local_addr(){
		return local_addr;
	}
	sockaddr_in &conn::get_remote_addr(){
		return remote_addr;
	}

	void conn::incr_buffer_ptr(
		int v){

		inbuf_ptr += v;
	}
};};
