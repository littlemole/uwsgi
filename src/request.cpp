#include "request.h"


Request::Request(wsgi_request* r)
    : r_(r)
{}

std::vector<Cookie> Request::cookies()
{
    std::vector<Cookie> v = Cookie::parse( get("HTTP_COOKIE") );
    return v;
}


std::string Request::method() 
{
    return get("REQUEST_METHOD");
}


std::string Request::content_type() 
{
    return get("CONTENT_TYPE");
}

size_t Request::content_length()
{
    std::string s = get("CONTENT_LENGTH");
    if ( s.empty() ) 
    {
        return 0;
    }
    
    std::istringstream iss(s);
    size_t result = 0;
    iss >> result;
    return result;
}

std::string Request::accept() 
{
    return get("HTTP_ACCEPT");
}

std::string Request::querystring() 
{
    return get("QUERY_STRING");
}
  
std::string Request::url() 
{
    return get("REQUEST_URI");
}

std::string Request::path() 
{
    return get("PATH_INFO");
}
          
std::string Request::body()
{
    std::ostringstream oss;
    ssize_t size = 4096;
    ssize_t len = 0;
    while( len == size)
    {
        char* body = uwsgi_request_body_read(r_, size , &len);
        if ( body )
        {
            oss << std::string(body,len);
        }
    }
    return oss.str();
}    

std::string Request::get(const std::string& key)
{
    uint16_t len = 0;
    char* p = uwsgi_get_var(r_, (char *) key.c_str(), key.size(), &len);
    return std::string(p,len);    
}
    
void Request::attr( const std::string& key, boost::any& a ) {
    attrs_.insert( std::make_pair( key, a ) );
}

boost::any Request::attr( const std::string& key ) {
    if ( attrs_.count(key) == 0 ) {
        return boost::any(nullptr);
    }
    return attrs_[key];
}

bool Request::hasAttr( const std::string& key )
{
    return attrs_.count(key) > 0;
}

QueryParams Request::queryParams()
{
    return QueryParams(querystring());
}

void Request::set_pathargs(const patharguments_t& args)
{
    args_ = args;
}

patharguments_t Request::path_info()
{
    return args_;
}


Args Request::args()
{
    return Args(args_);
}


