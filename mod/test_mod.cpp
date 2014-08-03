#include "registry.h"
#include "processor.h"

extern "C" struct uwsgi_cpp ucpp;

class Handler : public HttpHandler
{
public:

    Handler() : HttpHandler("GET","/test/.*")
    {}
    
    int request_handler( Request& req, Response& res ) 
    {
        std::string body = req.body();
//        res.status("200 OK");
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
        
        
        for ( Cookie c : req.cookies() )
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

/*
extern "C" int uwsgi_load_handler(Processor* proc)
{
    uwsgi_log("-------> load_handler\n");    
   // proc->registerHandler("GET","/test/",&handler);
    uwsgi_log("-------> loaded_handler\n");    
    return 1;
}
*/

/*
extern "C"  int test_request_handler(Request& req, Response& res) 
{
    std::string body = req.body();
    res.status("200 OK");
    res.header("Content-type", "text/plain");
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
    return res.ok();

/ *    ssize_t len = 0;
    char* body = uwsgi_request_body_read(wsgi_req, 4096 , &len);
    uwsgi_response_prepare_headers(wsgi_req, "200 OK", 6);
    uwsgi_response_add_header(wsgi_req, "Content-type", 12, "text/plain", 10);
    
    int i = 0;
    for ( i = 0; i < wsgi_req->var_cnt; i++ )
    {
         uwsgi_response_write_body_do(wsgi_req, (char*)wsgi_req->hvec[i].iov_base, wsgi_req->hvec[i].iov_len);
         uwsgi_response_write_body_do(wsgi_req, "\r\n", 2);
    }
    
    if (wsgi_req->post_cl == 0 ) 
    {
        uwsgi_response_write_body_do(wsgi_req, "ZERO", 4);
    }

    if ( wsgi_req->post_readline_buf )
    {
        uwsgi_response_write_body_do(wsgi_req, wsgi_req->post_readline_buf, strlen(wsgi_req->post_readline_buf));
        
    }
    if ( wsgi_req->post_read_buf && wsgi_req->post_read_buf_size > 0 )
    {
        uwsgi_response_write_body_do(wsgi_req, wsgi_req->post_read_buf, wsgi_req->post_read_buf_size);
    }
    uwsgi_response_write_body_do(wsgi_req, body, len);
    uwsgi_response_write_body_do(wsgi_req, wsgi_req->buffer, wsgi_req->uh->pktsize);
    return UWSGI_OK;
}
    */




