#pragma once

#include "choco/choco.h"
#include "packet.h"

class card_server : public choco::intf::handler{
public:
	card_server();
	virtual ~card_server();

	virtual int on_connected(
		choco::session::conn *client);
	virtual void on_disconnected(
		choco::session::conn *client);

	_async void on_login_request(
		choco::session::conn *client,
		login_request *pkt);

	void send_login_response(
		choco::intf::sendable *to,
		INT(result),
		CSTRING(nickname));
};