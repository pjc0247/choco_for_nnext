open_uri
----

지정된 uri의 문서를 string형으로 가져옵니다.

```C++
errorno err;
string doc;

err = http::open_uri(
  "http://www.google.com", doc);
```

* 반환하는 에러
- 
