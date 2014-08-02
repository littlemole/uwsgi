#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_DEF_GUARD_

#include "common.h"
#include "queryparams.h"
#include "arg.h"

class Request
{
public:

    Request(wsgi_request* wsgi_req);
        
    std::string method();
    
    QueryParams queryParams();
    Args args();

    std::string body();

    std::string accept();
    std::string content_type();
    size_t content_length();    

    std::string url(); 
    std::string path(); 
    std::string querystring();
    
    std::string get(const std::string& key);
    
    void attr( const std::string& key, boost::any& a );
    boost::any attr( const std::string& key );
    bool hasAttr( const std::string& key );
    
    void set_pathargs(const patharguments_t& args);
    patharguments_t path_info();
    
private:
    wsgi_request* r_; 
    std::map<std::string,boost::any> attrs_;
    patharguments_t args_;
};


#endif

