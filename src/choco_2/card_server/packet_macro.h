#pragma once

#define PACKET(name) \
	struct name : choco::packet::header{\
		name(){ \
			memset(this, 0, sizeof(this)); \
			id = id_##name; \
			size = sizeof(name); \
		};
#define END };

/* type macro */
#define STRING(name) char name[MAX_##name+1]
#define CSTRING(name) const char name[MAX_##name+1]
#define INT(name) int name
#define FLOAT(name) float name
#define CHAR(name) char name

/* max-length */
#define MAX_user_id 32
#define MAX_user_pw 32
#define MAX_nickname 32