#include "handler.h"
#include "response.h"

using namespace mol::whiskey;

class Handler : public HttpHandler
{
public:

    Handler() : HttpHandler("GET","/test/.*")
    {}
    
    int request_handler( Request& req, Response& res ) 
    {
        std::string body = req.body();
        res.contentType("text/plain");
        res.body("HELLO WORLD\r\n");
        res.body(body);
        res.body("\r\n");    
        res.body(req.querystring());
        res.body("\r\n");    
        res.body(req.path());
        res.body("\r\n");    
        auto params = req.queryParams();
        res.body(params.get("key a"));
        res.body("\r\n");   
        res.body("-----\r\n");   
        res.body(req.get("HTTP_COOKIE"));
        res.body("\r\n");   
        
        
        for ( Cookie c : req.cookies().all() )
        {
            res.body("got cookie: ");
            res.body(c.name());
            res.body(":");
            res.body(c.value());
            res.body(":");
            res.body(c.path());            
            res.body(":");
            res.body(c.domain());            
            res.body("\r\n");    
            res.cookie(c);
        } 
        
        
        res.body(req.path());        
        res.body("\r\n");    
        return res.ok();    
    }
};

static Handler handler;

class Interceptor : public HttpInterceptor
{
public:

    Interceptor() : HttpInterceptor("GET","/test/admin")
    {}
    
    int request_handler( Request& req, Response& res ) 
    {
        return res.done().bad_request();
    };
};

static Interceptor myinterceptor;


