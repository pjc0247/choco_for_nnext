#pragma once

#include "choco/packet/packet.h"

namespace choco{
namespace cluster{

	struct header : packet::header{
		unsigned long long packet_id;
	};
};};