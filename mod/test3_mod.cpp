#include "handler.h"
#include "response.h"

#define PASTE2(x,y) x##y
#define PASTE(x,y) PASTE2(x,y)
#define GET static auto PASTE(handler,__LINE__) = Get
#define POST static auto PASTE(handler,__LINE__) = Post
#define PUT static auto PASTE(handler,__LINE__) = Put
#define DELETE static auto PASTE(handler,__LINE__) = Delete
#define INTERCEPTOR static auto PASTE(handler,__LINE__) = interceptor

GET( "/foobar/")(
[] ( Request& req, Response& res ) {

    req.ws_handshake();    
    std::cerr << "ENTER WS LOOP " <<  std::endl;
    while(true)
    {
        std::string s = req.ws_recv();
        
        if(s.empty())
        {
            break;
        }
        std::cerr << "WS: " << s << std::endl;
        res.ws_send(s);    
    }
    std::cerr << "WS closed: " << std::endl;
    return res.ok();            
});




/*
INTERCEPTOR("GET","/test/admin2", 
[] (Request& req, Response& res) {
        return res.done().status("404 not found");
});

*/

