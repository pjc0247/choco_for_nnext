#pragma once

#include "choco/choco.h"
#include "packet_macro.h"

enum packet_id{
	id_login_request=1,
	id_login_response,
	id_foo,
	id_bar
};

PACKET(login_request)
	STRING(user_id);
	STRING(user_pw);
END

PACKET(login_response)
	INT(result);
	STRING(nickname);
END

PACKET(foo)
	INT(a);
END

PACKET(bar)
	INT(a);
END