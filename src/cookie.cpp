
#include "cookie.h"
#include <cstring>

Cookie::Cookie()
    :secure_(false)
{}

Cookie::Cookie(const std::string& name, const std::string& value)
    : name_(name), value_(value), secure_(false)
{}    


Cookie& Cookie::name(const std::string& value)
{
    name_ = value;
    return *this;
}

Cookie& Cookie::value(const std::string& value)
{
    value_ = value;
    return *this;
}

Cookie& Cookie::expires(const std::string& value)
{
    expires_ = value;
    return *this;
}

Cookie& Cookie::domain(const std::string& value)
{
    domain_ = value;
    return *this;
}

Cookie& Cookie::path(const std::string& value)
{
    path_ = value;
    return *this;
}

Cookie& Cookie::secure()
{
    secure_ = true;
    return *this;
}    

std::string Cookie::name()
{
    return name_;
}

std::string Cookie::value()
{
    return value_;
}

std::string Cookie::expires()
{
    return expires_;
}

std::string Cookie::domain()
{
    return domain_;
}

std::string Cookie::path()
{
    return path_;
}

bool Cookie::isSecure()
{
    return secure_;
}  

std::string Cookie::str()
{
    std::ostringstream oss;
    oss << name_ << "=" << value_;
    if ( !expires_.empty() )
    {
        oss << ";expires=" << expires_;
    }
    if ( !domain_.empty() )
    {
        oss << ";domain=" << domain_;
    }
    if ( !path_.empty() )
    {
        oss << ";path=" << path_;
    }
    if ( secure_ )
    {
        oss << ";secure";
    }
    return oss.str();
}

Cookie Cookie::parseCookie(const std::string& txt)
{
    Cookie cookie;
    std::vector<std::string> v = split(txt,';');
    if ( v.empty() )
    {
        return cookie;
    }
    
    std::vector<std::string> n = split(v[0],'=');
    if ( n.empty() )
    {
        return cookie;
    }
    
    cookie.name(n[0]);
    cookie.value(n[1]);
    
    for ( size_t i = 1; i < v.size(); i++)
    {
        std::string s = v[i];
        if ( strcasecmp(s.c_str(),"secure") == 0 )
        {
            cookie.secure();
            continue;
        }
        
        std::vector<std::string> n = split(s,'=');
        if ( n.empty() )
        {
            continue;
        }     
               
        if ( strcasecmp(n[0].c_str(),"expires") == 0 )
        {
            cookie.expires(n[1]);
            continue;
        }
        
        if ( strcasecmp(n[0].c_str(),"domain") == 0 )
        {
            cookie.domain(n[1]);
            continue;
        }
        
        if ( strcasecmp(n[0].c_str(),"path") == 0 )
        {
            cookie.path(n[1]);
            continue;
        }
    }
    return cookie;
}    

std::vector<Cookie> Cookie::parse(const std::string& txt)
{
    std::vector<Cookie> result;
    std::vector<std::string> v = split(txt,' ');
    for ( size_t i = 0; i < v.size(); i++)
    {
        Cookie cookie = parseCookie(v[i]);
        result.push_back(cookie);
    }
    return result;
}
