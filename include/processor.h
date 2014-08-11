#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_PROCESSOR_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_PROCESSOR_DEF_GUARD_

#include "handler.h"

class HandlerInfo
{
public:
    
    HandlerInfo( const std::string& m,  const std::string& p, HttpHandler* handler );
    
    bool match(const std::string& m, const std::string& p, patharguments_t& result);

    int invoke(Request& req, Response& res);
        
private:
    std::string method_;
    boost::regex path_regex_;    
    std::vector<std::string> args_;
    HttpHandler* handler_;
};

class InterceptorInfo
{
public:
    
    InterceptorInfo( const std::string& m,  const std::string& p, HttpHandler* handler );
    
    bool match(const std::string& m, const std::string& p);

    int invoke(Request& req, Response& res);
        
private:
    std::string method_;
    boost::regex path_regex_;    
    HttpHandler* handler_;
};
   
   
class Processor : public HttpHandler
{
public:

    Processor();
    
    Processor& registerHandler( const std::string& method, const std::string& path, HttpHandler* handler);
    Processor& registerInterceptor( const std::string& method, const std::string& path, HttpHandler* handler);

    int request_handler( Request& req, Response& res );

    Processor(const Processor& rhs) = delete;
    Processor& operator=(const Processor& rhs) = delete;
private:

    int intercept( Request& req, Response& res );

    std::vector<HandlerInfo> handlers_;
    std::vector<InterceptorInfo> interceptors_;
};


HttpHandler* processor();


 
#endif

