#include "handler.h"
#include "response.h"
#define HTTPHANDLER(h,l) h##l
#define POST(p) static auto HTTPHANDLER(handler,__LINE__) = Post(p)
#define GET(p) static auto HTTPHANDLER(handler,__LINE__) = Get(p)

//static auto handler = Get( "/test/{var}", [] ( Request& req, Response& res ) {

GET("/test/{var}")(
[]( Request& req, Response& res ) {

    res.status("200 OK");
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
    res.body(args.get("{var}"));
    res.body("\r\n");    
    
    return res.ok();            
});


