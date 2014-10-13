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

choco::server::server *sv;

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

	card_server *intf = new card_server();
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
		Sleep(2000);
	};

	return 0;
}

