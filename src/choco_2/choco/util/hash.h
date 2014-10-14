#pragma once

#include <string>

namespace choco{
namespace util{

	unsigned int hash(
		const std::string &str);
	unsigned int hash(
		void *data, int len);
};};