#pragma once

#include <string>

#include "choco/error/errno.h"

namespace choco{
namespace http{

	error open_uri(
		const std::string &uri,
		std::string &dst);
};};