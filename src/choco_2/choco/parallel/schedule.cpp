#include "schedule.h"

#include <Windows.h>
#include <thread>
#include <atomic>
#include <concurrent_unordered_map.h>
#include <queue>
#include <mutex>

#include "async.h"

#include "choco/log/log.h"
#include "choco/error/errno.h"

using namespace std;
using namespace concurrency;

namespace choco{
namespace parallel{

	struct scheduled_task{
		std::function<void()> task;
		int interval;
		int tick;
		int life;
		bool first_run;

		scheduled_task(){
			first_run = true;
		}
	};

	thread *scheduler_thread = nullptr;
	atomic<int> min_interval;
	mutex lock;
	vector<scheduled_task> tasks;

	void scheduler(){
		ULONGLONG tick;

		while(true){
			tick = GetTickCount64();
			Sleep(min_interval);

			unique_lock<mutex> guard(lock);
			for(auto it=tasks.begin();it!=tasks.end();){
				scheduled_task &task = *it;
				bool erase_it = false;

				if(task.tick <= 0){
					parallel::async(task.task);
					task.tick += task.interval;

					/* first exec */
					if(task.first_run){
						task.tick = 0;
						task.first_run = false;
					}
					/* decr life */
					if(task.life > 0){
						task.life--;

						if(task.life == 0){
							erase_it = true;
						}
					}

					if(task.tick < 0){
						log::warn(
							"parallel::scheduler - task delayed / delayed : %d\n",
							task.tick);
					}
				}
				
				task.tick -=
					(int)(GetTickCount64() - tick);

				if(erase_it)
					it = tasks.erase(it);
				else
					++it;
			}
		}
	}

	error initialize_scheduler(){
		min_interval = 100;

		scheduler_thread =
			new thread(scheduler);

		log::system(
			"parallel::scheduler - initialized\n");

		return errorno::none;
	}
	void cleanup_scheduler(){
		/* force terminate */
		if(scheduler_thread != nullptr){
			TerminateThread(
				scheduler_thread->native_handle(), 0);
			scheduler_thread = nullptr;
		}

		tasks.clear();
	}

	void schedule(
		std::function<void()> task,
		int interval){

		scheduled_task st;
		st.tick = st.interval = interval;
		st.task = task;
		st.life = -1;

		if(min_interval > interval)
			min_interval = interval;

		unique_lock<mutex> guard(lock);
			tasks.push_back(st);
	}
	void schedule_once(
		std::function<void()> task,
		int interval){

		scheduled_task st;
		st.tick = st.interval = interval;
		st.task = task;
		st.life = 1;

		if(min_interval > interval)
			min_interval = interval;

		unique_lock<mutex> guard(lock);
			tasks.push_back(st);
	}
	void unschedule(
		std::function<void()> task){

		unique_lock<mutex> guard(lock);
			for(auto it=tasks.begin();it!=tasks.end();++it){
				if(it->task.target<void()>()
					== task.target<void()>()){
					tasks.erase(it);
					break;
				}
			}
	}
};};