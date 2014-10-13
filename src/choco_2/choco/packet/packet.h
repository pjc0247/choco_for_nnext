#pragma once

#define _PACKET(name) \
	struct name : public choco::packet::header

namespace choco{
namespace packet{

#pragma pack (push, 1)
	struct header{
		int size;
		int id;
	};

	struct dummy : header{
		int dmy;
	};
#pragma pack (pop)
};};