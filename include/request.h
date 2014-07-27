#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_DEF_GUARD_

#include "common.h"
#include "queryparams.h"

class Request
{
public:

    Request(wsgi_request* wsgi_req);
        
    std::string method();
    std::string content_type();
    size_t content_length();
    std::string accept();
    std::string url(); 
    std::string path(); 
    std::string querystring();
    std::string body();
    
    std::string get(const std::string& key);
    
    QueryParams queryParams();
        
    void attr( const std::string& key, boost::any& a );
    boost::any attr( const std::string& key );
    bool hasAttr( const std::string& key );
    
private:
    wsgi_request* r_; 
    std::map<std::string,boost::any> attrs_;
};


#endif

