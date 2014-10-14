async
----

작업을 thread-pool에서 실행하도록 합니다.

```C++
/* 작업 */
void func(){
    Sleep(1000);
    printf("hello world\n");
}

/* func는 thread-pool에서 실행됩니다. */
parallel::async(func);
```
