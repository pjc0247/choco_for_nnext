#include "pool.h"

#include "choco/config/config.h"
#include "choco/config/keys.h"
#include "choco/log/log.h"

#define MALLOC(size, dst) do{\
	dst = ::malloc(size+4); \
	if(dst != nullptr) \
		memset(dst, size, 4); \
	} while(0)
#define MAKE_POOL(size) do{\
	for(int i=0;i<pool_##size;i++){ \
		void *ptr; \
		MALLOC(size, ptr); \
		if(ptr == nullptr) \
			return errorno::no_memory; \
		m##size.push(ptr); \
	} \
	} while(0)
#define CLEAN_POOL(size) do{\
	while(m##size.empty()){ \
		free(m##size.front()); \
		m##size.pop(); \
	}; \
	} while(0)

#define POOL_MALLOC(size, pool_size) do{\
	if(size >= pool_size){ \
		void *ptr; \
		if(m##pool_size.empty()) \
			MALLOC(pool_size, ptr); \
		else{ \
			ptr = m##pool_size.front(); \
			m##pool_size.pop(); \
		} \
		return ptr; \
	}} while(0)
#define POOL_FREE(ptr, size, pool_size) do{\
	if(size == pool_size){ \
		m##pool_size.push(ptr); \
	} \
	} while(0)

namespace choco{
namespace mem{

	pool::pool(){
	}
	pool::~pool(){
	}

	error pool::initialize(){
		int pool_32, pool_64, pool_128,
			pool_256, pool_512;

		_CONFIG_INT(mem_pool_32, pool_32);
		_CONFIG_INT(mem_pool_64, pool_64);
		_CONFIG_INT(mem_pool_128, pool_128);
		_CONFIG_INT(mem_pool_256, pool_256);
		_CONFIG_INT(mem_pool_512, pool_512);

		MAKE_POOL(32); MAKE_POOL(64);
		MAKE_POOL(128); MAKE_POOL(256);
		MAKE_POOL(512);

		return errorno::none;
	}
	void pool::cleanup(){
		CLEAN_POOL(32); CLEAN_POOL(64);
		CLEAN_POOL(128); CLEAN_POOL(256);
		CLEAN_POOL(512);
	}

	void *pool::malloc(size_t size){
		POOL_MALLOC(size, 32);
		POOL_MALLOC(size, 64);
		POOL_MALLOC(size, 128);
		POOL_MALLOC(size, 256);
		POOL_MALLOC(size, 512);

		log::error(
			"mem::pool - cannot allocate memory / size %d\n",
			size);

		return nullptr;
	}
	void pool::free(void *ptr){
		int size = *((char*)ptr - 4);

		POOL_FREE(ptr, size, 32);
		POOL_FREE(ptr, size, 64);
		POOL_FREE(ptr, size, 128);
		POOL_FREE(ptr, size, 256);
		POOL_FREE(ptr, size, 512);

		log::error(
			"mem::pool - cannot free memory / ptr : %x, size : %d\n",
			ptr, size);
	}
};};