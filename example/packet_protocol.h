#pragma once

#include "choco/choco.h"

enum packet_id{
    id_login_request,
    id_login_response
};

LENGTH(user_id, 32);
LENGTH(user_pw, 32);
LENGTH(nickname, 16);

PACKET(login_request)
    STRING(user_id);
    STRING(user_pw);
END

PACKET(login_response)
    INT(result);
    STRING(user_nickname);
END
