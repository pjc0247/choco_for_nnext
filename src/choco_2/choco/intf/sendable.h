#pragma once

namespace choco{
namespace intf{

	class sendable{
	public:
		sendable();
		virtual ~sendable();

		virtual int send(
			void *data,int len) =0;
	};
};};