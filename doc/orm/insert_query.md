INSERT 쿼리
====
```C++
auto person =
  orm::from("people")->create();

person->set("name", "kim young hee");
person->set("age", "18");
person->set("job", "student");

person->save();
```
