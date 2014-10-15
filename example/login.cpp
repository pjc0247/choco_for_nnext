#include "choco/choco.h"

#include "packet_protocol.h"

using namespace choco;

class login_test : public intf::handler{
public:
    login_test(){
        _ROUTE_ASYNC(login_test, login_request);
    }
    
    virtual void on_connected(
        session::conn *client){
    }
    virtual void on_disconnected(
        session::conn *client){
    }
    
    _async void on_login_request(
        session::conn *client,
        login_request *pkt){
        
        auto result = orm::from("account")
            ->where("id", pkt->id)
            ->where("pw", pkt->pw)
            ->find_one_async();
        
        if(result == nullptr){
            send_login_response(
                client,
                false, "");
        }
        else{
            send_login_response(
                client,
                true, result->get("nickname").c_str());
        }
    }
    
    void send_login_response(
        intf::sendable *to,
        INT(result), STRING(user_nickname)){
        
        login_response pkt;
        pkt.result = result;
        strcpy_s(pkt.user_nickname, user_nickname);
        
        to->send(&pkt, sizeof(login_response));
    }
};
