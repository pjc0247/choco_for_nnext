#include "parser.h"

#include <string>
#include <regex>

using namespace std;

namespace choco{
namespace config{
namespace parser{

	error process_line(
		const string &line,
		string &section,
		map<string,string> &dst){

		static regex re_section(
			"\\[([a-zA-Z0-9_\\.]+)\\]");
		static regex re_keyvalue(
			"([a-zA-Z0-9_\\.]+)\\s*=\\s*([a-zA-Z0-9_\\.]+)");

		smatch md;

		if(regex_match(line, md, re_section)){
			section = md[1].str();
		}
		else if(regex_match(line, md, re_keyvalue)){
			/* section_name::key = value */
			dst[section + "::" + md[1].str()] = 
				md[2].str();
		}

		return errorno::none;
	}
	error parse(
		const string &doc,
		map<string,string> &dst){

		string section;

		/* each_line */
		int nl_pos = 0;
		bool quit = false;
		while(!quit){
			auto at = 
				doc.find("\n", nl_pos+1);

			if(at == string::npos)
				quit = true;
			
			process_line(
				doc.substr(nl_pos+1, at-nl_pos-1),
				section, dst);
			nl_pos = at;
		}

		return errorno::none;
	}
};};};