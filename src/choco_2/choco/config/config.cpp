#include "config.h"

#include "choco/log/log.h"

#include "parser.h"

using namespace std;

namespace choco{
namespace config{

	map<string,string> configs;

	error from_file(
		const string &path){

		FILE *fp = fopen(path.c_str(), "r");
		if(fp == nullptr)
			return errorno::file_not_found;

		string doc;
		while(true){
			if(feof(fp))
				break;

			char buf[256];
			fgets(buf, 256, fp);

			doc.append(buf);
		}
		fclose(fp);

		return parser::parse(
			doc, configs);
	}
	error from_string(
		const string &str){

		return parser::parse(
			str, configs);
	}

	void set(
		const string &key,
		const string &value){

		configs[key] = value;
	}
	void set(
		const string &key,
		int value){

		char buffer[32];
		sprintf(buffer, "%d", value);

		set(key, buffer);
	}
	error get(
		const string &key,
		string &value){

		return get_as_string(
			key, value);
	}
	error get_as_string(
		const string &key,
		string &value){

		auto it = configs.find(key);
		if(it == configs.end())
			return errorno::key_not_exists;

		value = it->second;

		return errorno::none;
	}
	error get_as_int(
		const string &key,
		int &value){

		int ret;
		string s_value;
		ret = get_as_string(key, s_value);

		if(ret != 0)
			return ret;

		sscanf(s_value.c_str(), "%d", &value);

		return errorno::none;
	}


	void print(){
		log::output(
			"CONFIGS\n");
		for(auto config : configs){
			log::output(
				"  %-30s : %35s\n",
				config.first.c_str(), config.second.c_str());
		}
		log::output("\n");
	}
};};

