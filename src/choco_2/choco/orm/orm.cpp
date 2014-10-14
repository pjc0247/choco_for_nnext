#include "orm.h"

#include <memory>
#include <stdarg.h>

#include "choco/log/log.h"

using namespace std;

namespace choco{
namespace orm{

	/* from orm/config.cpp */
	error initialize_config();

	/* from orm/pool.cpp */
	error initialize_pool();
	void cleanup_pool();

	error initialize(){
		error ret;
		ret = initialize_config();
		_RETURN_ERR(ret);

		ret = initialize_pool();
		_RETURN_ERR(ret);

		return errorno::none;
	}
	void cleanup(){
		cleanup_pool();
	}

	string get_last_error(){
		MYSQL *mysql = get_db();

		return mysql_error( mysql );
	}
	int raw_query(const std::string &query){
		return mysql_query(
			get_db(), query.c_str() );
	}
	int raw_query(const std::string format, ...){
		va_list va;
		std::unique_ptr<char[]> query;
		int size = format.size() * 2; /* guessed size */

		while( true ){
			query.reset( new char [size] );

			va_start( va, format );
			int real = vsnprintf(
				&query[0],size,
				format.c_str(), va );
			va_end( va );

			if( real >= size )
				size += abs( real - size + 1);
			else
				break;
		}

		return mysql_query(
			get_db(), query.get() );
	}

	string escape(const string &str){
		static MYSQL *mysql = mysql_init( NULL );
		char escaped[1024];

		mysql_real_escape_string(
			mysql, escaped, str.c_str(), str.length() );

		return escaped;
	}

};};
