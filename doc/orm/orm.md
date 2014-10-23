orm
====

SQL을 이용하지 않고 DB작업을 수행하며,<br>
DB상의 row, column과 프로그래밍 언어의 오브젝트를 연결해 주는 것.


ORM의 이용
----
__일반적인 SQL의 이용__
```C++
const string query = 
  "SELECT nickname, level FROM accounts"
  "WHERE id=? AND password=?;"
  
q = db_prepare(query);
db_bind_param_str(q, user_id);
db_bind_param_str(q, user_password);

r = db_execute(q);

string user_nickname = 
  db_fetch_result_str(r);
int level = 
  db_fetch_result_int(r);
  
db_free_result(r);
db_free_query(q);
```
<br>
__ORM의 이용__
```C++
auto r = orm::from("accounts")
  ->where("id", user_id)
  ->where("password", user_pw)
  ->select(2, "nickname", "level")
  ->find_one();
  
cout<< r->get("nickname");
cout<< r->get("level");

delete r;
```
