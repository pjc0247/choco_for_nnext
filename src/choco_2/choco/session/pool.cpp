#include "pool.h"

#include "choco/log/log.h"

namespace choco{
namespace session{

	pool::pool(){
	}
	pool::~pool(){
	}

	error pool::initialize(){
		for(int i=0;i<POOL_MAX;i++){
			conns.push_back(
				new conn());
		}

		log::system(
			"session::pool - initialized / size : %d\n",
			POOL_MAX);

		return errorno::none;
	}
	void pool::cleanup(){
		for(auto conn : conns)
			delete conn;
		conns.clear();
	}

	pool::iterator pool::begin(){
		return conns.begin();
	}
	pool::iterator pool::end(){
		return conns.end();
	}
};};