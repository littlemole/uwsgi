#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_PROCESSOR_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_PROCESSOR_DEF_GUARD_

#include "common.h"
#include <map>
#include <memory>
#include <iostream>

class HandlerInfo
{
public:
    
    HandlerInfo( const std::string& m,  const std::string& p, HttpHandler* handler );
    
    bool match(const std::string& m, const std::string& p, patharguments_t& result);

    int invoke(Request& req, Response& res);
    
private:
    std::string method_;
    std::vector<std::string> path_;
    HttpHandler* handler_;
};
   
   
class Processor : public HttpHandler
{
public:

    Processor();
    
    Processor& registerHandler( const std::string& method, const std::string& path, HttpHandler* handler);

    int request_handler( Request& req, Response& res );
    

private:
    std::vector<HandlerInfo> handlers_;
};



HttpHandler* processor();


 
#endif

