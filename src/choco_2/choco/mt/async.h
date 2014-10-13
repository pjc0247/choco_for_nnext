#pragma once

#include <functional>
#include <future>

#include "microthread.h"

#include "choco/parallel/async.h"
#include "choco/server/server.h"

#define _async

namespace choco{
namespace mt{

	template <typename T>
	T async(
		server::server *s,
		std::function<T(void)> task){

		T result;
		microthread *mt = 
			get_current_microthread();

		parallel::async(
			[=, &result](){
				result = task();
				s->notify_task_complete(
					mt, nullptr);
		});

		mt->yield();

		return result;
	}
};};

/*
	mt::async<orm::query*(
		[](){
			return orm::where("id","pjc")
				->find_one();
		});
*/