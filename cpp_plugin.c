
#include "uwsgi.h"
#include <dlfcn.h>
#include <map>
#include <string>
#include <sstream>

extern struct uwsgi_server uwsgi;

static int uwsgi_cpp_init()
{
    uwsgi_log("i am the example plugin initialization function\n");
    return 0;
}

extern "C" {

  typedef int(*handler_t)(struct wsgi_request *wsgi_req);
}

static std::map<std::string,handler_t> handlers;

static int uwsgi_cpp_request(struct wsgi_request *wsgi_req) 
{
	if (uwsgi_parse_vars(wsgi_req)) 
	{
        return -1;
    }
    
    uint16_t len = 0;
    char* path_info = uwsgi_get_var(wsgi_req, (char *) "PATH_INFO", 9, &len);
    if(!path_info)
    {
        return -1;
    }

    std::string ctx(path_info,len);
    while (!ctx.empty() && ctx[0] == '/' ) {
        ctx = ctx.substr(1);
    }
    
    while (!ctx.empty() && (ctx[ctx.size()-1] == '\n' || ctx[ctx.size()-1] == '\r') ) {
        ctx = ctx.substr(0,ctx.size()-2);
    }    
    
    uwsgi_log("handler: %s\n", ctx.c_str());    
    
    if ( handlers.count(ctx) == 0 )
    {
        handlers[ctx] = 0;
        std::ostringstream oss;
        oss << "/home/mike/uwsgi/" << ctx << "_mod.so";
        std::string soname = oss.str();
        uwsgi_log("load .so: %s\n", soname.c_str());
        void* handle = dlopen(soname.c_str(), RTLD_LAZY);
        
        if (handle) 
        {
            dlerror();
            std::ostringstream oss;
            oss << ctx << "_request_handler";   
            std::string handlername = oss.str();        
            uwsgi_log("load func: %s\n", handlername.c_str()); 
            handler_t h = (handler_t) dlsym(handle,handlername.c_str());
            const char *dlsym_error = dlerror();
            if (!dlsym_error && h) 
            {
                uwsgi_log("loaded func: %s\n", handlername.c_str()); 
                 handlers[ctx] = h;
            }
        }
    }
    
    handler_t handler = handlers[ctx];
    if (!handler) 
    {
        return -1;
    }
    
    int r = handler(wsgi_req);
    return r;
    return UWSGI_OK;
/*

    ssize_t len = 0;
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
    */
}


static void uwsgi_cpp_after_request(struct wsgi_request *wsgi_req) 
{
    uwsgi_log("i am the example plugin after request function\n");
}


struct uwsgi_plugin cpp_plugin = {

        .name = "example",
        .alias = 0,
        .modifier1 = 250,
        .data = 0,
        .on_load = 0,
        .init = uwsgi_cpp_init,
        .post_init = 0,
        .post_fork = 0,
        .options = 0,
        .enable_threads = 0,
        .init_thread = 0,
        .request = uwsgi_cpp_request,
        .after_request = uwsgi_cpp_after_request

};


