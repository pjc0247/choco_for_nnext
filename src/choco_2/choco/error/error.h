#pragma once

#define _RETURN_ERR(e) {\
	if(e != errorno::none) \
		return e; \
	} while(0);

namespace choco{
	typedef int error;
};