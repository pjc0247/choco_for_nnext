#include "pool.h"

#include <Windows.h>

#include "worker.h"

#include "choco/log/log.h"
#include "choco/config/config.h"
#include "choco/config/keys.h"

namespace choco{
namespace server{
namespace worker{

	pool::pool(){
	}
	pool::~pool(){
	}

	int pool::initialize(server *server){
		int pool_size;
		int ret;

		ret = config::get_as_int(
			config::keys::server_pool_size, pool_size);
		_RETURN_ERR(ret);

		auto bound_method = 
			std::bind(func, server);
		for(int i=0;i<pool_size;i++){
			workers.push_back(
				std::thread(bound_method));
		}

		log::system(
			"server::worker::pool - initialized / size : %d\n",
			pool_size);

		return errorno::none;
	}
	void pool::cleanup(){
		for(auto &worker : workers){
			worker.detach();
			TerminateThread(
				worker.native_handle(), 0);
		}
		workers.clear();
	}

};};};