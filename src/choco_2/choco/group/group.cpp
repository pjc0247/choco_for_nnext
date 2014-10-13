#include "group.h"

#include "choco/session/conn.h"

#include <concurrent_queue.h>

namespace choco{

	group::group(){
	}
	group::~group(){
	}

	void group::add_child(
		group *g){

		children.push_back(g);
	}
	void group::remove_child(
		group *g){

		concurrency::concurrent_queue<int> q;

		concurrency::concurrent_unordered_set<int> w;
		
		
	
	}

	int group::send(
		void *data,int len){

		return 0;
	}
};