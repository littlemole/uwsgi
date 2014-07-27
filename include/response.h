#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_RESPONSE_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_RESPONSE_DEF_GUARD_
#include "request.h"

class Response
{
public:

    Response(wsgi_request* wsgi_req);
        
    Response& status(const std::string& s);
    Response& header(const std::string& key, const std::string& val);
    Response& body(const std::string& b);
    int ok();
    
private:
    wsgi_request* r_; 
};


#endif

