#include "orm.h"

using namespace std;

namespace choco{
namespace orm{

	string join(vector<string> vec, string delim){
		string result;

		if( vec.empty() )
			return "";

		for(auto it=vec.begin();it!=vec.end()-1;++it){
			result +=
				(*it) + delim;
		}

		return result +
			(*vec.rbegin());
	}

	string query::build_result_columns(){
		if( results.empty() )
			return "*";
		else
			return join( results, "," );
	}
	string query::build_conditions(){
		string query = "WHERE ";

		return query +
			join( conditions, " AND " );
	}
	string query::build_field_keys(){
		string keys;
		auto it = fields.begin();

		while( it != fields.end() ){
			keys += (*it).first;

			if( ++it != fields.end() )
				keys += ",";
		}

		return keys;
	}
	string query::build_field_values(){
		string values;
		auto it = fields.begin();

		while( it != fields.end() ){
			values += "\'" + 
				escape((*it).second) + "\'";

			if( ++it != fields.end() )
				values += ",";
		}

		return values;
	}
	string query::build_changes(){
		string changes;

		if( dirty_fields.empty() )
			return "";

		for(auto field_name : dirty_fields){
			changes +=
				field_name + "=\'" +
				escape(get(field_name)) + "\',";
		}
		changes.pop_back();

		return changes;
	}
	string query::build_order_by(){
		string orders = "ORDER BY ";

		if( order_by.empty() )
			return "";

		orders +=
			join( order_by, "," );

		return orders;
	}
	string query::build_group_by(){
		string groups = "GROUP BY ";

		if( group_by.empty() )
			return "";

		groups +=
			join( group_by, "," );

		return groups;
	}

	string query::build_from(){
		string query = "FROM ";

		query += table;
	
		return query;
	}
	string query::build_into(){
		string query = "INTO ";

		query += table;
	
		return query;
	}
	string query::build_limit(){
		string query = "LIMIT ";
		char buf[12];

		if( n_limit == -1 )
			return "";

		sprintf( buf, "%d", n_limit );
		query += string(buf);
	
		return query;
	}
	string query::build_offset(){
		string query = "OFFSET ";
		char buf[12];

		if( n_offset == -1 )
			return "";

		sprintf( buf, "%d", n_offset );
		query += string(buf);

		return query;
	}

	string query::build_select(){
		string query = "SELECT ";

		if( b_distinct )
			query += "DISTINCT ";

		query +=
			build_result_columns() + " " +
			build_from() + " " +
			build_conditions() + " " +
			build_order_by() + " " +
			build_limit() + " " +
			build_offset();

		return query;
	}
	string query::build_update(){
		string query = "UPDATE ";

		query +=
			table + " SET " +
			build_changes() + " " +
			build_conditions();
		return query;
	}
	string query::build_delete(){
		string query = "DELETE ";

		query +=
			build_from() + " " +
			build_conditions();
		return query;
	}
	string query::build_insert(){
		string query = "INSERT ";
		query +=
			build_into() + " (" +
			build_field_keys() + ") VALUES (" +
			build_field_values() +")";

		return query;
	}

	string query::build(){
		switch( query_type ){
		case eSELECT:
			return build_select();
		case eUPDATE:
			return build_update();
		case eDELETE:
			return build_delete();
		case eINSERT:
			return build_insert();
		}
		return "";
	}

};};
