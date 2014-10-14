basic
----

간단한 시작 예제

```C++
#include "choco/choco.h"

using namespace choco;

class my_server : public intf::raw{
public:
  
  virtual void on_connected(
    session::conn *client){
  }
  virtual void on_disconnected(
    session::conn *client){
  }
  
  virtual void on_data(
    session::conn *client,
    void *data, int len){
    
    /* echo-back */
    client->send(data, len);
  }
};

int main(int argc,char **argv){
  
  if(config::from_file("config.cfg")){
    printf("config::from_file error\n");
    return -1;
  }
  
  if(initialize(init_log)){
    printf("initialize error\n");
    return -1;
  }
  
  auto handler = new my_server;
  auto server = new server::server(handler);
  
  if(server->open()){
    printf("server::open error\n");
    return -1;
  }
  
  while(!quit_flag){
    printf("running\n");
    Sleep(2000);
  }
  
  cleanup();
  
  return 0;
}
```
