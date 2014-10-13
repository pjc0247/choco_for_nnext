#include "pool.h"

#include <Windows.h>

#include "choco/log/log.h"
#include "choco/config/config.h"
#include "choco/config/keys.h"

using namespace std;

namespace choco{
namespace parallel{

	pool::pool() :
		running(false){
	}
	pool::~pool(){
	}

	error pool::initialize(){
		int ret;
		int pool_size;
		running = true;

		/* get pool_size from configs */
		ret = config::get_as_int(
			config::keys::parallel_pool_size, pool_size);
		if(ret != errorno::none)
			return ret;

		for(int i=0;i<pool_size;i++){
			auto bound_method = 
				bind(&pool::worker, this);

			workers.push_back(
				thread(bound_method));
		}

		log::system(
			"parallel::pool - initialized / size : %d\n",
			pool_size);

		return errorno::none;
	}
	void pool::cleanup(){
		running = false;

		/* force_terminate */
		for(thread &thread : workers){
			TerminateThread(
				thread.native_handle(), 0);
		}

		/* */
		/*
		signal.notify_all();
		*/

		workers.clear();
	}

	void pool::enqueue(
		function<void()> task){

		q.push(task);
		signal.notify_one();
	}
	function<void()> pool::dequeue(){
		while(true){
			function<void()> task;
			unique_lock<std::mutex> lock(mutex);
				if(q.empty())
					signal.wait(lock);
				q.try_pop(task);

			if(task == nullptr)
				continue;

			return task;
		}
	}

	void pool::worker(){
		while(running){
			auto task = dequeue();

			task();
		}
	}
};};