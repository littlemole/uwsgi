#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_DEF_GUARD_

#include "common.h"
#include "queryparams.h"
#include "arg.h"
#include "cookie.h"

namespace mol {
namespace whiskey {


class Request
{
public:

    Request(wsgi_request* wsgi_req);
        
    std::string method();
    
    QueryParams queryParams();
    Args args();
    Cookies& cookies();

    std::string body();

    std::string accept();
    std::string content_type();
    size_t content_length();    

    std::string url(); 
    std::string path(); 
    std::string querystring();
    
    std::string get(const std::string& key);    
    
    void setAttr( const std::string& key, const boost::any& a );
    boost::any getAttr( const std::string& key );
    bool hasAttr( const std::string& key );
    
    template<class T>
    void attr(const std::string& key, const T& a )
    {
        setAttr( key, boost::any(a) );
    }
    
    template<class T>
    T attr(const std::string& key)
    {
        return boost::any_cast<T>(getAttr(key));
    }
        
    void set_pathargs(const patharguments_t& args);
    patharguments_t path_info();
    
    void ws_handshake();
    std::string ws_recv();
    
private:
    wsgi_request* r_; 
    std::map<std::string,boost::any> attrs_;
    patharguments_t args_;
    Cookies cookies_;
};

} // end namespace whiskey
} // end namespace mol

#endif

