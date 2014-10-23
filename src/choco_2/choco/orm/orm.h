#pragma once

#include <WinSock2.h>
#include <mysql.h>

#include <string>
#include <vector>
#include <map>

#include "choco/error/errno.h"

namespace choco{
namespace server{
	class server;
};

namespace orm{

	class query;

	error initialize();
	void cleanup();

	/* ERROR API */
	std::string get_last_error();

	/* CONFIGURATION API */
	void configure(
		const std::string &key, const std::string &value);
	std::string &get_config(
		const std::string &key);

	/* TRANSACTION API */
	bool try_begin();
	void begin();
	void commit();
	void rollback();

	/* RAW API */
	MYSQL *get_db();
	int raw_query(const std::string &query);
	int raw_query(const std::string format, ...);

	/* SELECT TABLE */
	query *from(const std::string &table);

	/* ESCAPE */
	std::string escape(const std::string &str);

	class query{
		friend query *from(const std::string &table);

	public:
		query *where(const std::string &col, const std::string &value);
		query *where_equal(const std::string &col, const std::string &value);
		query *where_not_equal(const std::string &col, const std::string &value);
		query *where_like(const std::string &col, const std::string &value);
		query *where_not_like(const std::string &col, const std::string &value);
		query *where_gt(const std::string &col, const std::string &value);
		query *where_gte(const std::string &col, const std::string &value);
		query *where_lt(const std::string &col, const std::string &value);
		query *where_lte(const std::string &col, const std::string &value);
		query *where_raw(const std::string &query);

		query *select(const std::string &col);
		query *select(int count, ...);

		query *limit(int limit);
		query *offset(int offset);

		query *order_by_expr(const std::string &expr);
		query *order_by_asc(const std::string &col);
		query *order_by_desc(const std::string &col);

		void set(const std::string &key, const std::string &value);
		std::string &get(const std::string &key);

		query *find_one();
		std::vector<query*> find_many();

		std::string find_min(const std::string &col);
		std::string find_max(const std::string &col);
		std::string find_avg(const std::string &col);
		std::string find_sum(const std::string &col);

		/* async */
		query *find_one_async(
			server::server *s);
		std::vector<query*> find_many_async(
			server::server *s);

		std::string find_min_async(
			server::server *s,
			const std::string &col);
		std::string find_max_async(
			server::server *s,
			const std::string &col);
		std::string find_avg_async(
			server::server *s,
			const std::string &col);
		std::string find_sum_async(
			server::server *s,
			const std::string &col);

		query *create();
		bool save();
		bool remove();

		std::string build();

	public:
		std::string &operator[](const std::string &key);

	protected:
		query();
		virtual ~query();

		void set_connection_object(MYSQL *mysql);

		void set_with_no_dirt(const std::string &key, const std::string &value);

		void set_query_type(int query_type);
		void set_table(const std::string &table);
		void set_limit(int limit);
		void set_offset(int offset);

		void add_result_column(const std::string &col);
		void add_condition(
			const std::string &col, const std::string &op, const std::string &value);
		void add_condition(const std::string &condition);
		void add_order_by(const std::string &col, const std::string &order);
		void add_group_by(const std::string &col);

		std::string build_result_columns();
		std::string build_conditions();
		std::string build_field_keys();
		std::string build_field_values();
		std::string build_changes();
		std::string build_order_by();
		std::string build_group_by();

		std::string build_from();
		std::string build_into();

		std::string build_limit();
		std::string build_offset();
	
		std::string build_select();
		std::string build_update();
		std::string build_delete();
		std::string build_insert();

		void dirt_field(const std::string &field_name);
		void clean_dirty_fields();

		/* SELECT, UPDATE, INSERT, DELETE backends */
		std::string find_single_value();
		query *find_single_record();
		std::vector<query*> find_records();
		bool update_records();
		bool insert_record();
		bool remove_records();

		/* MYSQL Wrapping */
		int exec_query(const std::string &query_str);
		MYSQL_RES *store_result();
		void free_result(MYSQL_RES *mysql);
		std::vector<std::string> fetch_fields(MYSQL_RES *result);
		std::vector<MYSQL_ROW> fetch_rows(MYSQL_RES *result);
		MYSQL_ROW fetch_next_row(MYSQL_RES *result);

	protected:
		enum queryType{
			eSELECT=1,
			eUPDATE,
			eDELETE,
			eINSERT
		};

	protected:
		int query_type;

		std::string table;

		std::vector<std::string> conditions;
		std::vector<std::string> results;
		std::vector<std::string> dirty_fields;
		std::vector<std::string> order_by;
		std::vector<std::string> group_by;

		std::map<std::string,std::string> fields;

		int n_limit;
		int n_offset;
		bool currupted;

		MYSQL *mysql;
	};
};};