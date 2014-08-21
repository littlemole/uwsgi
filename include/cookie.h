#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_COOKIE_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_COOKIE_DEF_GUARD_

#include "common.h"

namespace mol {
namespace whiskey {

class Cookie
{
public:
    Cookie();
    Cookie(const std::string& name, const std::string& value);

    Cookie& name(const std::string& value);
    Cookie& value(const std::string& value);    
    Cookie& expires(const std::string& value);
    Cookie& maxAge(int seconds);    
    Cookie& domain(const std::string& value);
    Cookie& path(const std::string& value);
    Cookie& secure();
    
    std::string name() const;
    std::string value() const;
    std::string expires() const;
    int maxAge() const;
    std::string domain() const;
    std::string path() const;    
    bool isSecure() const;
        
    std::string str() const;
    
private:
    std::string name_;
    std::string value_;
    std::string expires_;
    std::string maxAge_;    
    std::string domain_;
    std::string path_;
    bool secure_;
};

class Cookies
{
public:

    Cookie parseCookie(const std::string& txt);
    std::vector<Cookie>& parse(const std::string& txt);

    bool exists(const std::string& name);
    const Cookie& get(const std::string& name);
    
    std::vector<Cookie>& all();
private:
    std::vector<Cookie> cookies_;
};

} // end namespace whiskey
} // end namespace mol

#endif
