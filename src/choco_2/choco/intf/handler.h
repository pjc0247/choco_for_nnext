#pragma once

#include <map>
#include <functional>

#include "choco/mt/microthread.h"
#include "choco/packet/packet.h"

#include "raw.h"

#define _BIND(func) \
	std::bind(func, this, std::placeholders::_1, std::placeholders::_2)

#define _ROUTE(klass, packet) {\
	route<packet>( \
		id_##packet, \
		std::bind(&##klass##::on_##packet, this, \
			std::placeholders::_1, std::placeholders::_2)); \
	} while(0);
#define _ROUTE_ASYNC(klass, packet) {\
	route_async<packet>( \
		id_##packet, \
		std::bind(&##klass##::on_##packet, this, \
			std::placeholders::_1, std::placeholders::_2)); \
	} while(0);

namespace choco{
namespace intf{

	class handler : public raw{
	public:
		handler();
		virtual ~handler();

		virtual int on_connected(
			session::conn *s);
		virtual void on_disconnected(
			session::conn *s);
		virtual void on_data(
			session::conn *s,
			void *data,int len);

	protected:

		template <typename T>
		void invoker(
			std::function<void(session::conn*,T*)> f,
			session::conn *client,
			packet::header* arg){

			f(client, (T*)arg);	
		}
		template <typename T>
		void invoker_async(
			std::function<void(session::conn*,T*)> f,
			session::conn *client,
			packet::header* arg){

			auto mt = new mt::microthread(
				[=](mt::microthread *mt){
					f(client, (T*)arg);
				});
			mt->schedule();
		}

		template <typename T>
		void route(
			int pid,
			std::function<void(session::conn*,T*)> callback){

			callbacks[pid] = std::bind(
				&handler::invoker<T>,
				this,
				callback, std::placeholders::_1, std::placeholders::_2);
		};

		template <typename T>
		void route_async(
			int pid,
			std::function<void(session::conn*,T*)> callback){

			callbacks[pid] = std::bind(
				&handler::invoker_async<T>,
				this,
				callback, std::placeholders::_1, std::placeholders::_2);
		};

	private:
		void process_header(
			session::conn *s, int len);
		void process_body(
			session::conn *s, int len);

	private:
		std::map<int,std::function<void(session::conn*,packet::header*)>> callbacks;
	};
};};