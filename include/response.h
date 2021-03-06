#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_RESPONSE_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_RESPONSE_DEF_GUARD_
#include "request.h"

namespace mol {
namespace whiskey {

class Response
{
public:

    Response(wsgi_request* wsgi_req);
        
    Response& contentType(const std::string& val);
    Response& header(const std::string& key, const std::string& val);
    Response& body(const std::string& b);
    Response& flush();
    Response& done();
    Response& reset();
    
    Response& cookie(const Cookie& c);    
        
    int status(const std::string& s);        
    int ok();
    int error();
    int bad_request();
    int unauthorized();            
    int forbidden();     
    int not_found();            
    int redirect(const std::string& s, int code = 302);
    
    std::string status();
    std::string body();
    headers_t headers();
    bool isDone();
    
    void ws_send(const std::string& s);
    
private:
    wsgi_request* r_; 
    std::string status_;
    std::string body_;
    headers_t headers_;
    bool done_;
    std::vector<Cookie> cookies_;
};

inline Response& operator<<(Response& lhs, const std::string& rhs)
{
    lhs.body(rhs);
    return lhs;
}

} // end namespace whiskey
} // end namespace mol

#endif

