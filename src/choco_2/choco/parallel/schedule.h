#pragma once

#include <functional>

namespace choco{
namespace parallel{

	void schedule(
		std::function<void()> task,
		int interval);
	void schedule_once(
		std::function<void()> task,
		int interval);

	void unschedule(
		std::function<void()> task);
};};