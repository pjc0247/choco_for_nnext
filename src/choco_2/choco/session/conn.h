#pragma once

#include <string>

#include "choco/common.h"
#include "choco/intf/sendable.h"
#include "choco/error/errno.h"

namespace choco{
namespace intf{
	class handler;
};

namespace session{

	class conn : public intf::sendable{
		friend intf::handler;
	
	public:
		conn();
		virtual ~conn();

		error open(
			int inbuf_size);
		void close();

		error accept(
			socket server_sock);

		int recv(
			int len);
		int recv_append(
			int len);
		virtual int send(
			void *data, int len);
		
		int set_opt(
			int level, int optname,
			const char *optval,int optlen);
		int get_opt(
			int level, int optname,
			char *optval,int *optlen);

		char *get_buffer();
		int get_buffer_ptr();
		socket get_socket();
		sockaddr_in &get_local_addr();
		sockaddr_in &get_remote_addr();

		void incr_buffer_ptr(
			int v);

	private:
		socket sock;
		sockaddr_in local_addr, remote_addr;
		event evt;

		char *inbuf;
		int inbuf_max, inbuf_ptr;

		int state; /* for intf::handler */
	};
};};