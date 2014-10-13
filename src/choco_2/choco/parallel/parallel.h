#pragma once

#include "choco/error/errno.h"

namespace choco{
namespace parallel{

	error initialize();
	void cleanup();
};};