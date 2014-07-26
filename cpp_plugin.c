
#include "uwsgi.h"
#include <dlfcn.h>

extern struct uwsgi_server uwsgi;

static int uwsgi_cpp_init()
{
    uwsgi_log("i am the example plugin initialization function\n");
    return 0;
}

static int uwsgi_cpp_request(struct wsgi_request *wsgi_req) 
{
	if (uwsgi_parse_vars(wsgi_req)) {
        return -1;
    }


    ssize_t len = 0;
    char* body = uwsgi_request_body_read(wsgi_req, 4096 , &len);
    uwsgi_response_prepare_headers(wsgi_req, "200 OK", 6);
    uwsgi_response_add_header(wsgi_req, "Content-type", 12, "text/plain", 10);
    
    int i = 0;
    for ( i = 0; i < wsgi_req->var_cnt; i++ )
    {
         uwsgi_response_write_body_do(wsgi_req, wsgi_req->hvec[i].iov_base, wsgi_req->hvec[i].iov_len);
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


static void uwsgi_cpp_after_request(struct wsgi_request *wsgi_req) 
{
    uwsgi_log("i am the example plugin after request function\n");
}


struct uwsgi_plugin cpp_plugin = {

        .name = "example",
        .modifier1 = 250,
        .init = uwsgi_cpp_init,
        .request = uwsgi_cpp_request,
        .after_request = uwsgi_cpp_after_request,

};


