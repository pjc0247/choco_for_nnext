// choco_2.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <map>

#include "choco/choco.h"
#include "card_server/server.h"

#pragma comment (lib,"ws2_32")
#pragma comment (lib,"MSWSock")
#pragma comment (lib,"libcurl")
#pragma comment (lib,"libmysql")

using namespace std;
using namespace choco;

choco::server::server *sv;

class minsoo : public intf::handler{
public:
	minsoo(){
		_ROUTE(minsoo, login_request);
		_ROUTE(minsoo, foo);
	}

	virtual int on_connected(
		session::conn *client){

		printf("접속받앗다\n");
		return 0;
	}
	virtual void on_disconnected(
		session::conn *client){
		printf("ㄲㅈ\n");
	}

	virtual void on_login_request(
		session::conn *client,
		login_request *pkt){


		login_response resp;
		if(strcmp(pkt->user_id, "user") == 0 &&
			strcmp(pkt->user_pw, "asdf1234") == 0){

			resp.result = true;
			strcpy(resp.nickname, "pjc0247");
			client->send(&resp, sizeof(login_response));
		}
		else{
			resp.result = false;
			client->send(&resp, sizeof(login_response));
		}
	}
	virtual void on_foo(
		session::conn *client,
		foo *pkt){

		bar b;
		b.a = pkt->a + 1;

		client->send(&b, sizeof(bar));
	}
};

int _tmain(int argc, _TCHAR* argv[]){

	map<string,string> dst;
	choco::config::from_file("config.cgf");
	choco::config::print();

	/*
		orm config는 옛날에 만든거 가져온거라
		전역 config랑 통합 아직 안됨.
	*/
	choco::orm::configure("user", "root");
	choco::orm::configure("password", "asdf1234");
	choco::orm::configure("db", "test");

	choco::initialize(
		choco::init_log | choco::init_parallel |
		choco::init_mysql);

	auto *intf = new minsoo();
	choco::server::server *server = new
		choco::server::server(intf);

	sv = server;
	
	if(server->open()){
		choco::log::error(
			"failed to open server\n");
		return 0;
	}


	while( true ){
		choco::log::output(
			"[running] / \n");
		Sleep(5000);
	};

	return 0;
}

