#include "orm.h"

#include <assert.h>

using namespace std;

namespace choco{
namespace orm{

	query::query() :
		n_limit(0),
		query_type(0),
		currupted(false){
	}
	query::~query(){
	}

	void query::set_with_no_dirt(const string &key, const string &value){
		fields[key] = value;
	}
	void query::set_connection_object(MYSQL *_mysql){
		mysql = _mysql;
	}

	void query::set_query_type(int _query_type){
		query_type = _query_type;
	}
	void query::set_table(const string &_table){
		table = _table;
	}
	void query::set_limit(int _limit){
		n_limit = _limit;
	}
	void query::set_offset(int _offset){
		n_offset = _offset;
	}

	void query::add_result_column(const string &col){
		results.push_back( col );
	}
	void query::add_condition(
		const string &col, const string &op, const string &value){

		conditions.push_back(
			col + " " + op + " \'" +
			escape(value) + "\'" );
	}
	void query::add_condition(const string &query){
		conditions.push_back( query );
	}
	void query::add_order_by(const string &col, const string &order){
		order_by.push_back(
			col + " " + order );
	}
	void query::add_group_by(const string &col){
		group_by.push_back( col );
	}

	int query::exec_query(const std::string &query_str){
		assert( currupted == false );

		int result = mysql_query(
			mysql, query_str.c_str() );

		currupted = true;

		return result;
	}
	MYSQL_RES *query::store_result(){
		MYSQL_RES *result = 
			mysql_store_result( mysql );

		return result;
	}
	void query::free_result(MYSQL_RES *mysql){
		mysql_free_result( mysql );
	}
	vector<string> query::fetch_fields(MYSQL_RES *result){
		vector<string> fields;
		int n_field =
			mysql_num_fields( result );

		for(int i=0;i<n_field;i++) {
			MYSQL_FIELD *field =
				mysql_fetch_field( result );
		
			fields.push_back(
				field->name );
		}

		return fields;
	}
	vector<MYSQL_ROW> query::fetch_rows(MYSQL_RES *result){
		vector<MYSQL_ROW> rows;
		my_ulonglong n_row =
			mysql_num_rows( result );
	
		for(my_ulonglong i=0;i<n_row;i++) {
			MYSQL_ROW row = 
				fetch_next_row( result );

			rows.push_back( row );
		}

		return rows;
	}
	MYSQL_ROW query::fetch_next_row(MYSQL_RES *result){
		MYSQL_ROW row =
			mysql_fetch_row( result );
	
		return row;
	}

	void query::dirt_field(const string &field_name){
		dirty_fields.push_back( field_name );
	}
	void query::clean_dirty_fields(){
		dirty_fields.clear();
	}

	string query::find_single_value(){
		string sql = build_select();

		if( exec_query( sql ))
			return "";

		MYSQL_RES *result = store_result();
		if( result == NULL )
			return "";

		MYSQL_ROW row =
			fetch_next_row( result );
		if( row == NULL )
			return "";
	
		mysql_free_result( result );

		return row[0];
	}
	query *query::find_single_record(){
		string sql = build_select();

		if( exec_query( sql ))
			return NULL;

		MYSQL_RES *result = store_result();
		if( result == NULL )
			return NULL;

		MYSQL_ROW row =
			fetch_next_row( result );
		if( row == NULL )
			return NULL;

		orm::query *obj = orm::from( table );
		auto fields =
			fetch_fields( result );

		for(unsigned int i=0;i<fields.size();i++)
			obj->set_with_no_dirt( fields[i], row[i] );

		/* 나중에 이 레코드를 식별하기 위해 */
		obj->where("id", obj->get("id") );
		obj->set_query_type(
			queryType::eUPDATE );
	
		mysql_free_result( result );

		return obj;
	}
	vector<query*> query::find_records(){
		vector<query*> results;
		string sql = build_select();

		if( exec_query( sql ))
			return results;

		MYSQL_RES *result = store_result();
		if( result == NULL )
			return results;

		auto fields =
			fetch_fields( result );
		auto rows =
			fetch_rows( result );

		for(unsigned int i=0;i<rows.size();i++){
			orm::query *obj = orm::from( table );

			for(unsigned int j=0;j<fields.size();j++)
				obj->set_with_no_dirt( fields[j], rows[i][j] );

			/* 나중에 이 레코드를 식별하기 위해 */
			obj->where("id", obj->get("id") );
			obj->set_query_type(
				queryType::eUPDATE );

			results.push_back( obj );
		}
	
		mysql_free_result( result );

		return results;
	}
	bool query::update_records(){
		string sql = build_update();

		if( exec_query( sql ))
			return false;
		return true;
	}
	bool query::insert_record(){
		string sql = build_insert();

		if( exec_query( sql ))
			return false;
		return true;
	}
	bool query::remove_records(){
		string sql = build_delete();

		if( exec_query( sql ))
			return false;
		return true;
	}

};};
