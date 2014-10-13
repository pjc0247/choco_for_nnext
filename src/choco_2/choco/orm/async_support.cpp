#include "orm.h"

#include "choco/mt/async.h"

using namespace std;

namespace choco{
namespace orm{

	query *query::find_one_async(
		server::server *s){

		return mt::async<query*>(
			s,
			[this](){
				return find_one();
			});
	}
	std::vector<query*> query::find_many_async(
		server::server *s){

		return mt::async<vector<query*>>(
			s,
			[this](){
				return find_many();
			});
	}

	std::string find_min_async(const std::string &col);
	std::string find_max_async(const std::string &col);
	std::string find_avg_async(const std::string &col);
	std::string find_sum_async(const std::string &col);
};};