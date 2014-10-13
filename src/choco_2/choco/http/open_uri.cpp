#include "open_uri.h"

#include <curl/curl.h>

using namespace std;

namespace choco{
namespace http{

	size_t write_data(
		void *buffer, size_t size,
		size_t nmemb, void *userp){

		size_t sizes = size*nmemb;

		((string*)userp)->append(
			(char*)buffer, 0,sizes);

		return sizes;
	}
	error open_uri(
		const string &uri,
		string &dst){

		CURL *curl = curl_easy_init();

		curl_easy_setopt(
			curl,
			CURLOPT_URL, uri.c_str());
		curl_easy_setopt(
			curl,
			CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(
			curl,
			CURLOPT_WRITEDATA, &dst);
		
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		return errorno::none;
	}
};};