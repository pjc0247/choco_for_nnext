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

	void func(
		server *sv){

		mt::initialize();
		while(true){
			BOOL ret;
			DWORD tranferred;
			DWORD key;
			event *evt;

			ret = GetQueuedCompletionStatus(
				sv->iocp,
				&tranferred,
				(PULONG_PTR)&key, (OVERLAPPED**)&evt,
				INFINITE);

			if(!ret)
				continue;

			switch(evt->type){
			case event_accept:{
				error ret = 
					sv->process_connection(evt->conn);
				if(ret != errorno::none){
					log::error(
						"server::worker - failed to accept / socket : %d, errorno : %d\n",
						sv->get_socket(), ret);
				}} break;
			case event_recv:
				sv->process_recv_completion(
					evt->conn, evt, tranferred);
				break;
			case event_task_complete:
				evt->mt->schedule();
				break;
			}
		}

		mt::cleanup();
	}
};};};