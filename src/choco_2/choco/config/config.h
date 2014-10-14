#pragma once

#include <string>
#include <map>

#include "choco/error/errno.h"

#define _CONFIG(key, dst, type) {\
	int ret = choco::config::get_as_##type( \
		choco::config::keys::##key, dst); \
	_RETURN_ERR(ret); \
	} while(0);
#define _CONFIG_STR(key, dst) _CONFIG(key,dst, string)
#define _CONFIG_INT(key, dst) _CONFIG(key,dst, int)

namespace choco{
namespace config{

	error from_file(
		const std::string &path);
	error from_string(
		const std::string &str);

	void set(
		const std::string &key,
		const std::string &value);
	void set(
		const std::string &key,
		int value);

	error get(
		const std::string &key,
		std::string &value);
	error get_as_string(
		const std::string &key,
		std::string &value);
	error get_as_int(
		const std::string &key,
		int &value);

	void print();

};};