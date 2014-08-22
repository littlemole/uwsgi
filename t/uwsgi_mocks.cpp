#include "common.h"
#include "uwsgi.h"
 
// uwsgi mocks

std::map<std::string,std::string> wsgi_env_mock;
std::string wsgi_body_mock;

// uwsgi spies

std::string wsgi_status_spy;
mol::whiskey::headers_t wsgi_headers_spy;
std::string wsgi_body_spy;


// uwsgi test double impl that uses above mocks and spies

extern "C" {

void uwsgi_log(const char* c, ...)
{

}

int uwsgi_parse_vars(wsgi_request* r)
{
    return 0;
}

char* uwsgi_request_body_read( wsgi_request* r, ssize_t s,ssize_t* len)
{
    *len = wsgi_body_mock.size();
    return (char*)wsgi_body_mock.c_str();
}

char* uwsgi_get_var(wsgi_request* r, char * s, uint16_t size,uint16_t* len)
{
    *len = 0;
    std::string key(s,size);

    if ( wsgi_env_mock.count(key) == 0 )
    {
        return 0;
    }    
    
    const std::string& val = wsgi_env_mock[key];
    *len = val.size();
    
    return (char*)val.c_str();
}

int uwsgi_response_prepare_headers(wsgi_request* r,char* s ,uint16_t size)
{
    wsgi_status_spy = std::string(s,size);
    return 0;
}

int uwsgi_response_add_header(wsgi_request* r,char* k ,uint16_t ks,char* v ,uint16_t vs)
{
    std::string key = std::string(k,ks);
    std::string val = std::string(v,vs);
    
    wsgi_headers_spy.push_back( mol::whiskey::header_t(key,val) );
    return 0;
}

int uwsgi_response_write_body_do(wsgi_request* r,char* s ,size_t size)
{
    wsgi_body_spy.append( std::string(s,size) );
    return 0;
}

void uwsgi_opt_set_str(char* c, char* s, void* v)
{
}

#if !defined(MOL_UWSGI_VERSION_2)
int uwsgi_websocket_handshake(struct wsgi_request *, char *, uint16_t, char *, uint16_t)
#else
int uwsgi_websocket_handshake(struct wsgi_request *, char *, uint16_t, char *, uint16_t, char*, uint16_t)
#endif
{
    return 0;
}

int uwsgi_websocket_send(struct wsgi_request *, char *, size_t)
{
    return 0;
}

struct uwsgi_buffer *uwsgi_websocket_recv(struct wsgi_request *)
{
    return 0;
}

void uwsgi_buffer_destroy(uwsgi_buffer* b)
{

}


} // end extern "C"

