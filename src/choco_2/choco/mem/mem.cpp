#include "mem.h"

#include "pool.h"

#include "choco/config/config.h"
#include "choco/config/keys.h"
#include "choco/log/log.h"

using namespace std;

namespace choco{
namespace mem{

	bool use_mem_pool = false;
	__declspec(thread) pool *p = nullptr;

	error print_log(){
		int pool_32, pool_64,
			pool_128, pool_256, pool_512;

		_CONFIG_INT(mem_pool_32, pool_32);
		_CONFIG_INT(mem_pool_64, pool_64);
		_CONFIG_INT(mem_pool_128, pool_128);
		_CONFIG_INT(mem_pool_256, pool_256);
		_CONFIG_INT(mem_pool_512, pool_512);

		log::system(
			"mem - initialized / %10dbyte(s) per thread\n",
			pool_32 * 36 + pool_64 * 68 +
			pool_128 * 132 + pool_256 * 260 + pool_512 * 516);

		return errorno::none;
	}

	error initialize(bool turn_on){
		if(turn_on){
			use_mem_pool = true;
			return print_log();
		}
		if(use_mem_pool){
			p = new(nothrow) pool();
			if(p == nullptr)
				return errorno::no_memory;

			return p->initialize();
		}
		else 
			return errorno::none;
	}
	void cleanup(){
		if(use_mem_pool){
			p->cleanup();
			delete p;
			p = nullptr;
		}
	}

	void *malloc(size_t size){
		if(p)
			return p->malloc(size);
		else
			return malloc(size);
	}
	void free(void *ptr){
		if(p)
			p->free(ptr);
		else
			return free(ptr);
	}
};};