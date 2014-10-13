#include "sleep.h"

#include "choco/server/server.h"
#include "choco/parallel/schedule.h"

#include "microthread.h"

namespace choco{
namespace mt{

	void sleep_async(
		server::server *s,
		int ms){

		microthread *mt = 
			get_current_microthread();

		parallel::schedule_once(
			[=](){
				s->notify_task_complete(
					mt, nullptr);
			},
			ms);

		mt->yield();
	}
};};