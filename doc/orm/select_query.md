SELECT 쿼리
====
__싱글 레코드에 대한 쿼리__
```C++
orm::from("people")
  ->where("name", "kim chul soo")
  ->find_one();
```
__1개 이상의 레코드에 대한 쿼리__
```C++
auto results = orm::from("people")
  ->where("age", "20")
  ->find_many();
  
for(auto result : results)
  cout<< result->get("name")<< endl;
```
__갯수 제한, 오프셋__
```C++
orm::from("people")
  ->where("age", "20")
  ->limit(10)
  ->offset(5)
  ->find_many();
```
__결과 정렬__
```C++
orm::from("people")
  ->where("age", "20")
  ->order_by_asc("weight")
  ->find_many();
```
