#include "handler.h"
#include "response.h"

using namespace mol::whiskey;

#define PASTE2(x,y) x##y
#define PASTE(x,y) PASTE2(x,y)
#define GET static auto PASTE(handler,__LINE__) = Get
#define POST static auto PASTE(handler,__LINE__) = Post
#define PUT static auto PASTE(handler,__LINE__) = Put
#define DELETE static auto PASTE(handler,__LINE__) = Delete
#define INTERCEPTOR static auto PASTE(handler,__LINE__) = interceptor

GET( "/test2/{var}")(
[] ( Request& req, Response& res ) {

    res.header("Content-type", "text/plain");
    
    res.body("HELLO2 WORLD\r\n");
    res.body(req.body());
    res.body("\r\n");    
    
    res.body(req.querystring());
    res.body("\r\n");    
    
    res.body(req.path());
    res.body("\r\n");    
    
    auto params = req.queryParams();
    res.body(params.get("key a"));
    res.body("\r\n");    
    
    auto args = req.args();
    res.body(args.get("var"));
    res.body("\r\n");    
    
    return res.ok();            
});


GET("/")(
[]( Request& req, Response& res ) {

    res.header("Content-type", "text/plain");    
    res.body("ROOT\r\n");    
    return res.ok();            
});

INTERCEPTOR("GET","/test/admin2", 
[] (Request& req, Response& res) {
        return res.done().status("404 not found");
});



