#include "hash.h"

using namespace std;

namespace choco{
namespace util{

	unsigned int hash(
		const string &str){

		unsigned int hash = 0;
		for(int i=0;i<str.length();i++)
			hash = 65599 * hash + str[i];
		return hash ^ (hash >> 16);
	}
	unsigned int hash(
		void *data, int len){

		char *cdata = (char*)data;
		unsigned int hash = 0;
		for(int i=0;i<len;i++)
			hash = 65599 * hash + cdata[i];
		return hash ^ (hash >> 16);
	}
};};