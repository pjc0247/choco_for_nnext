DELETE 쿼리
====
```C++
auto person = orm::from("people")
  ->where("name", "kim chul soo")
  ->find_one();
  
person->remove();
```
```C++
auto person = orm::from("people")
  ->where("age", "20")
  ->remove();
```
