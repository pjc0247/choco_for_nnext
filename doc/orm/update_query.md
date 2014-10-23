UPDATE 쿼리
====
```C++
auto person = orm::from("people")
  ->where("name", "kim chul soo")
  ->find_one();
  
person->set("age", "25");

person->save();
```
