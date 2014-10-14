#include "parallel.h"

#include "pool.h"

namespace choco{
namespace parallel{

	error initialize_scheduler();
	void cleanup_scheduler();

	pool *worker_pool = nullptr;

	error initialize(){
		int ret;

		worker_pool = new pool();
		ret = worker_pool->initialize();
		_RETURN_ERR(ret);

		ret = initialize_scheduler();
		_RETURN_ERR(ret);

		return errorno::none;
	}
	void cleanup(){
		if(worker_pool != nullptr){
			delete worker_pool;
			worker_pool = nullptr;
		}

		cleanup_scheduler();
	}
};};