#include "server.h"

using namespace choco;

card_server::card_server(){
	/*
	route_async<login_request>(
		id_login_request,
		std::bind(&card_server::on_login_request, this,
		std::placeholders::_1, std::placeholders::_2));
		*/

	//_ROUTE_ASYNC(card_server, login_request);
}
card_server::~card_server(){
}

int card_server::on_connected(
	session::conn *client){
	handler::on_connected(client);

	log::info(
		"new connection\n");
	return true;
}
void card_server::on_disconnected(
	session::conn *client){
	handler::on_disconnected(client);

	log::info(
		"disconnected\n");
}