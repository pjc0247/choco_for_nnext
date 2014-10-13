#include "orm.h"

#include <cstdarg>

using namespace std;

namespace choco{
namespace orm{

	query *from(const string &table){
		query *query = new orm::query();
		
			query->set_table( table );

		return query;
	}

	query *query::where(const string &col, const string &value){
		add_condition( col, "=", value );

		return this;
	}
	query *query::where_equal(const string &col, const string &value){
		add_condition( col, "=", value );

		return this;
	}
	query *query::where_not_equal(const string &col, const string &value){
		add_condition( col, "!=", value );

		return this;
	}
	query *query::where_like(const string &col, const string &value){
		add_condition( col, "LIKE", value );

		return this;
	}
	query *query::where_not_like(const string &col, const string &value){
		add_condition( col, "NOT LIKE", value );

		return this;
	}
	query *query::where_gt(const string &col, const string &value){
		add_condition( col, ">", value );

		return this;
	}
	query *query::where_gte(const string &col, const string &value){
		add_condition( col, ">=", value );

		return this;
	}
	query *query::where_lt(const string &col, const string &value){
		add_condition( col, "<", value );

		return this;
	}
	query *query::where_lte(const string &col, const string &value){
		add_condition( col, "<=", value );

		return this;
	}

	query *query::where_raw(const string &query){
		add_condition( query );

		return this;
	}

	query *query::select(const string &col){
		add_result_column( col );

		return this;
	}
	query *query::select(int count, ...){
		va_list va;

		va_start( va, count );
			for(int i=0;i<count;i++){
				add_result_column(
					va_arg( va, const string ));
			}
		va_end( va );

		return this;
	}

	query *query::limit(int limit){
		set_limit( limit );

		return this;
	}

	query *query::order_by_expr(const string &expr){
		add_order_by( expr, "" );

		return this;
	}
	query *query::order_by_asc(const string &col){
		add_order_by( col, "ASC" );

		return this;
	}
	query *query::order_by_desc(const string &col){
		add_order_by( col, "DESC" );

		return this;
	}

	query *query::create(){
		query *query = orm::from( table );
		query->set_query_type(
			queryType::eINSERT );

		return query;
	}

	query *query::find_one(){
		set_limit( 1 );
		set_connection_object( get_db() );

		return find_single_record();
	}
	vector<query*> query::find_many(){
		set_connection_object( get_db() );

		return find_records();
	}

	string query::find_min(const string &col){
		set_connection_object( get_db() );

		results.clear();
		add_result_column(
			"MIN(" + col + ")" );

		return find_single_value();
	}
	string query::find_max(const string &col){
		set_connection_object( get_db() );

		results.clear();
		add_result_column(
			"MAX(" + col + ")" );

		return find_single_value();
	}
	string query::find_avg(const string &col){
		set_connection_object( get_db() );

		results.clear();
		add_result_column(
			"AVG(" + col + ")" );

		return find_single_value();
	}
	string query::find_sum(const string &col){
		set_connection_object( get_db() );

		results.clear();
		add_result_column(
			"SUM(" + col + ")" );

		return find_single_value();
	}

	bool query::remove(){
		set_connection_object( get_db() );

		return remove_records();
	}
	bool query::save(){
		set_connection_object( get_db() );

		bool ret;

		/* UPDATE query */
		if( query_type == queryType::eUPDATE )
			ret = update_records();
		/* INSERT query */
		else
			ret = insert_record();

		clean_dirty_fields();

		return ret;
	}

	void query::set(const string &key, const string &value){
		dirt_field( key );
		fields[key] = value;
	}
	string &query::get(const string &key){
		return fields[key];
	}
	string &query::operator[](const std::string &key){
		/* operator[]로 접근된 값들은 수정 여부를 알 수 없으므로
		   무조건 dirt시킨다, 나중에 수정 */
		dirt_field( key );

		return fields[key];
	}

};};
