#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_HANDLER_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_HANDLER_DEF_GUARD_
#include "common.h"

class Processor;
class Request;
class Response;

class HttpHandler
{
public:

    HttpHandler() {}
    
    HttpHandler(const std::string& m, const std::string& p);

    virtual int request_handler( Request& req, Response& res )  = 0;
}; 


#endif

