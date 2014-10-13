#pragma once

namespace choco{
namespace session{
	class conn;
};

namespace intf{
	class raw{
	public:
		raw();
		virtual ~raw();

		virtual int on_connected(
			session::conn *s) =0;
		virtual void on_disconnected(
			session::conn *s) =0;
		virtual void on_data(
			session::conn *s,
			void *data,int len) =0;
	};
};};