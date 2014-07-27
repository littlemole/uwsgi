#include "response.h"


Response::Response(wsgi_request* r)
    : r_(r)
{}

        
Response& Response::status(const std::string& s)
{
    uwsgi_response_prepare_headers(
        r_, 
        (char*)s.c_str(), s.size()
    );
    return *this;
}

Response& Response::header(const std::string& key, const std::string& val)
{
    uwsgi_response_add_header(
        r_, 
        (char*)key.c_str(), key.size(), 
        (char*)val.c_str(), val.size()
    );
    return *this;
}

Response& Response::body(const std::string& b)
{
    uwsgi_response_write_body_do(
        r_, 
        (char*)b.c_str(), b.size()
    );
    return *this;
}

int Response::ok()
{
    return UWSGI_OK;
}



