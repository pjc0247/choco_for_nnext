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
		server::server *_sv,
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
		sv = _sv;

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

		inbuf_ptr = 0;
		sv->process_recv(
			this, &evt,
			inbuf, len);

		return true;
	}
	int conn::recv_append(
		int len){

		if(inbuf_max <= inbuf_ptr+len)
			return false;

		sv->process_recv(
			this, &evt,
			inbuf + inbuf_ptr, len);
			
		return true;
	}
	int conn::send(
		void *data,int len){

		sv->process_send(
			this, &evt,
			data, len);

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

	void *conn::get_user_data(){
		return user_data;
	}
	void conn::set_user_data(
		void *_user_data){

		user_data = _user_data;
	}
};};
