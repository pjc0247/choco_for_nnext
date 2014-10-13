#pragma once

#include <WinSock2.h>
#include <Windows.h>

namespace choco{
	namespace session{
		class conn;
	}
	namespace mt{
		class microthread;
	};
	
	typedef SOCKET socket;
	typedef SOCKADDR sockaddr;
	typedef SOCKADDR_IN sockaddr_in;

	//typedef OVERLAPPED event;
	struct event : OVERLAPPED{
		char type;
		void *data;

		mt::microthread *mt;
		session::conn *conn;
	};
};