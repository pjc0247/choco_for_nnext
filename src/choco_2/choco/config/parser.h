#pragma once

#include <string>
#include <vector>
#include <map>

#include "choco/error/errno.h"

namespace choco{
namespace config{
namespace parser{

	error parse(
		const std::string &doc,
		std::map<std::string,std::string> &dst);

};};};