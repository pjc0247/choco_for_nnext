#pragma once

#include <string>
#include <map>

#include "choco/error/errno.h"

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