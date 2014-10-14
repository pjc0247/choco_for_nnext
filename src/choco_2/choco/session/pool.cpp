#include "pool.h"

#include "choco/log/log.h"
#include "choco/config/config.h"
#include "choco/config/keys.h"

namespace choco{
namespace session{

	pool::pool() :
		size(0), accepted(0){
	}
	pool::~pool(){
	}

	error pool::initialize(){
		int ret;
		int inbuf_size;
		int pool_size;

		ret = config::get_as_int(
			config::keys::session_inbuf_size,
			inbuf_size);
		if(ret != 0)
			return ret;

		ret = config::get_as_int(
			config::keys::session_pool_size,
			pool_size);
		if(ret != 0)
			return ret;

		size = 0;
		for(int i=0;i<pool_size;i++,size++){
			conn *c = new conn();
			
			conns.push_back(c);
			ret = c->open(inbuf_size);

			if(ret != 0)
				return ret;
		}

		log::system(
			"session::pool - initialized / size : %d\n",
			pool_size);

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

	int pool::get_size(){
		return size;
	}
	int pool::get_accepted(){
		return accepted;
	}
};};