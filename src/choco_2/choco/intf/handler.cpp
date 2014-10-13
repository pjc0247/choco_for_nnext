#include "handler.h"

#include <deque>

#include "choco/log/log.h"
#include "choco/session/conn.h"

using namespace std;

namespace choco{
namespace intf{

	enum conn_state{
		recv_header=0,
		recv_body
	};

	handler::handler(){
	}
	handler::~handler(){
	}

	int handler::on_connected(
		session::conn *client){
			
		client->recv(4);
		return 0;
	}
	void handler::on_disconnected(
		session::conn *client){
	}

	void handler::process_header(
		session::conn *s, int len){

		int target_len =
			offsetof(packet::header, id);
		int received_len =
			s->get_buffer_ptr();

		/* target_len만큼 수신 */
		if(received_len == target_len){
			int packet_size =
				*(int*)s->get_buffer();

			s->recv_append(
				packet_size	- target_len);
			s->state = recv_body;
		}
		/* 재수신 요청 */
		else{
			s->recv_append(
				target_len - received_len);
		}
	}
	void handler::process_body(
		session::conn *s, int len){

		int target_len =
			*(int*)s->get_buffer();
		int received_len =
			s->get_buffer_ptr();

		/* target_len만큼 수신 */
		if(received_len == target_len){
			packet::header header =
				*(packet::header*)s->get_buffer();

			auto it = callbacks.find(header.id);
			if(it != callbacks.end()){
				it->second(
					s,
					(packet::header*)s->get_buffer());
			}
			/* 등록되지 않은 패킷 */
			else{
				log::system(
					"intf::handler - unregistered packet / id : %d\n",
					header.id);
			}
		}
		/* 재수신 요청 */
		else{
			s->recv_append(
				target_len - received_len);
		}
	}

	void handler::on_data(
		session::conn *s,
		void *data,int len){

		switch(s->state){
		case recv_header:
			process_header(s, len);
			break;
		case recv_body:
			process_body(s, len);
			break;
		}
	}
};};

