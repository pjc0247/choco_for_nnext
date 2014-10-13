#include "server.h"

using namespace choco;

extern server::server *sv;

_async void card_server::on_login_request(
	session::conn *client,
	login_request *pkt){

	printf("begin\n");

	printf("before %d\n", GetTickCount());
	mt::sleep_async(sv, 1000);
	printf("after %d\n", GetTickCount());

	/* mt delete Ã³¸® */

	auto r = mt::async<orm::query*>(
		sv,
		[pkt](){
			orm::begin();
			auto r =  orm::from("account")
				->where("id", pkt->user_id)
				->where("password", pkt->user_pw)
				->find_one();
			orm::commit();
			return r;
		});

	printf("after job %s\n", r->get("nickname").c_str());

		/*
	orm::begin();
	printf("hello\n");
	auto result = orm::from("account")
		->where("id", pkt->user_id)
		->where("pw", pkt->user_pw)
		->find_one_async(sv);

	printf("after db\n");

	if(result == nullptr){
		printf("account exist\n");
		send_login_response(
			client,
			false, "");
	}
	else{
		printf("account not exist\n");
		send_login_response(
			client,
			true, result->get("nickname").c_str());
	}
	orm::commit();
	*/
}

void card_server::send_login_response(
	choco::intf::sendable *to,
	INT(result),
	CSTRING(nickname)){

	login_response resp;

	strncpy(
		resp.nickname,
		nickname, sizeof(nickname));
	resp.result = result;

	to->send(&resp, sizeof(resp));
}


/*
	mt::sleep(
		[](mt::signal *s){
			s->wake_up();
		});
*/