#pragma once

#include "choco/choco.h"

#define MAX_user_id 32
#define MAX_user_pw 32
#define MAX_nickname 16

enum packet_id{
    id_login_request,
    id_login_response
};

PACKET(login_request)
    STRING(user_id);
    STRING(user_pw);
END

PACKET(login_response)
    INT(result);
    STRING(user_nickname);
END
