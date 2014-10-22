#pragma once

#include "choco/error/errno.h"

namespace choco{
namespace mem{

	error initialize(bool turn_on = false);
	void cleanup();

	void *malloc(size_t size);
	void free(void *ptr);
};};