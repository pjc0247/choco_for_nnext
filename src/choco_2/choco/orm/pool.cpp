#include "orm.h"

#include <thread>
#include <concurrent_queue.h>

#include <mysql.h>

#include "choco/log/log.h"
#include "choco/config/config.h"
#include "choco/config/keys.h"

using namespace std;
using namespace chrono;
using namespace concurrency;

namespace choco{
namespace orm{

	concurrent_queue<MYSQL*> conns;
	_declspec(thread) MYSQL *local_db = nullptr;

	error initialize_pool(){
		int port, pool_size;
		string host, user, password, db;

		_CONFIG_INT(mysql_pool_size, pool_size);
		_CONFIG_STR(mysql_host, host);
		_CONFIG_INT(mysql_port, port);
		_CONFIG_STR(mysql_user, user);
		_CONFIG_STR(mysql_password, password);
		_CONFIG_STR(mysql_db_name, db);

		for(int i=0;i<pool_size;i++){
			MYSQL *mysql = mysql_init(NULL);

			if( mysql_real_connect(
				mysql,
				host.c_str(), user.c_str(), password.c_str(), 
				db.c_str(), port, NULL, NULL) == 0 ){
			
				/* connection error */
				mysql_close( mysql );				
				return false;
			}

			mysql_autocommit( mysql, false );

			conns.push( mysql );
		}

		log::system(
			"orm::pool - initialized / size : %d\n",
			pool_size);

		return errorno::none;
	}
	void cleanup_pool(){
		while( !conns.empty() ){
			MYSQL *mysql;
	
			if( conns.try_pop( mysql ) )
				mysql_close( mysql );
		}
	}

	void return_db(MYSQL *mysql){
		local_db = NULL;
		conns.push( mysql );
	}

	bool try_begin(){
		MYSQL *mysql;

		if( conns.empty() )
			return false;
		if( !conns.try_pop( mysql ) )
			return false;
		else{
			local_db = mysql;
			mysql_query( mysql, "BEGIN" );

			return true;
		}
	}
	void begin(){
		MYSQL *mysql;

		while( true ){
			if( !conns.empty() ){
				if( conns.try_pop( mysql ) )
					break;
			}

			this_thread::sleep_for(
				milliseconds(10) );
		}

		local_db = mysql;
		mysql_query( mysql, "BEGIN" );
	}
	void commit(){
		MYSQL *mysql = local_db;

		mysql_query( mysql, "COMMIT" );

		return_db( mysql );
	}
	void rollback(){
		MYSQL *mysql = local_db;

		mysql_query( mysql, "ROLLBACK" );

		return_db( mysql );
	}

	MYSQL *get_db(){
		return local_db;
	}

};};
