#pragma once

#include <functional>

namespace choco{
namespace mt{

	class microthread{
	public:
		microthread();
		microthread(
			std::function<void(microthread*)> f);
		virtual ~microthread();

		void yield();
		void schedule();

	private:
		static void _stdcall fiber_func(void *arg);

	private:
		std::function<void(microthread*)> func;
		void *yield_fiber, *fiber;
	};

	microthread *get_current_microthread();
};};

/*

microthread(

*/