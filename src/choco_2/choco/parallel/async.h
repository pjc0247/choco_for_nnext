#pragma once

#include <functional>
#include <future>

namespace choco{
namespace parallel{

	void async(
		std::function<void()> task);

	template <typename T>
	std::future<T> async(
		std::function<T()> task){

		std::future<T> future;		
		return future;
	}
};};