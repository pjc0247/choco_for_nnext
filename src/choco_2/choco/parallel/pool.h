#pragma once

#include <functional>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <thread>
#include <concurrent_queue.h>

#include "choco/error/errno.h"

namespace choco{
namespace parallel{

	class pool{
	public:
		pool();
		virtual ~pool();

		error initialize();
		void cleanup();

		void enqueue(
			std::function<void()> task);
		
	protected:
		void worker();
		std::function<void()> dequeue();

	private:
		concurrency::concurrent_queue<std::function<void()>> q;
		std::mutex mutex;
		std::condition_variable signal;

		std::vector<std::thread> workers;

		bool running;
	};
};};