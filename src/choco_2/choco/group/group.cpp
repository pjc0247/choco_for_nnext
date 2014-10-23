#include "group.h"

#include "choco/session/conn.h"

#include <concurrent_queue.h>

namespace choco{

	group::group(){
	}
	group::~group(){
	}

	void group::join(
		intf::sendable *s){

		children.push_back(s);
	}
	void group::remove(
		intf::sendable *s){

		for(auto &child : children){
			if(child == s)
				child = nullptr;
		}
	}

	int group::send(
		void *data,int len){

		for(auto &child : children){
			if(child != nullptr)
				child->send(data, len);
		}

		return 0;
	}
};