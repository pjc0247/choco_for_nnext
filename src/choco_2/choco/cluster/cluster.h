#pragma once

#include "choco/error/errno.h"

namespace choco{
namespace cluster{

	error send(
		int cluster_id,
		void *data, int len);
};};