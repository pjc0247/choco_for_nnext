#include "microthread.h"

#include <Windows.h>

using namespace std;

namespace choco{
namespace mt{	

	microthread::microthread() : 
		fiber(nullptr){
	}
	microthread::microthread(
		function<void(microthread*)> f) :
		func(f), fiber(nullptr){

		fiber = CreateFiber(
			0,
			fiber_func, this);
	}
	microthread::~microthread(){
		if(fiber != nullptr)
			DeleteFiber(fiber);
	}

	void _stdcall microthread::fiber_func(void *arg){
		microthread *m = (microthread*)arg;

		m->func(m);
		m->yield();
	}

	void microthread::yield(){
		SwitchToFiber(yield_fiber);
	}
	void microthread::schedule(){
		yield_fiber = GetCurrentFiber();
		SwitchToFiber(fiber);
	}

	microthread *get_current_microthread(){
		return (microthread*)GetFiberData();
	}
};};