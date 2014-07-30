#include "handler.h"
#include "response.h"

class MyHandler : public HttpHandler
{
public:

    MyHandler() : HttpHandler("POST","/test2/{var}")
    {}
    
    int request_handler( Request& req, Response& res ) 
    {
        std::string body = req.body();
        res.status("200 OK");
        res.header("Content-type", "text/plain");
        res.body("HELLO2 WORLD\r\n");
        res.body(body);
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
    }
};

static MyHandler myhandler;

