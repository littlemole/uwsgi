#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_COOKIE_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_COOKIE_DEF_GUARD_

#include "common.h"

class Cookie
{
public:
    Cookie();
    Cookie(const std::string& name, const std::string& value);

    Cookie& name(const std::string& value);
    Cookie& value(const std::string& value);    
    Cookie& expires(const std::string& value);
    Cookie& domain(const std::string& value);
    Cookie& path(const std::string& value);
    Cookie& secure();
    
    std::string name();
    std::string value();
    std::string expires();
    std::string domain();
    std::string path();    
    bool isSecure();
        
    std::string str();
    
    static Cookie parseCookie(const std::string& txt);
    static std::vector<Cookie> parse(const std::string& txt);  
private:
    std::string name_;
    std::string value_;
    std::string expires_;
    std::string domain_;
    std::string path_;
    bool secure_;
};

#endif
