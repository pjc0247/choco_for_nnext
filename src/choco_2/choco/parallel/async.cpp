#include "async.h"

#include "pool.h"

using namespace std;

namespace choco{
namespace parallel{

	extern pool *worker_pool;

	void async(
		function<void()> task){

		worker_pool->enqueue(task);
	}
};};