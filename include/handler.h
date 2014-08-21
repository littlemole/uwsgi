#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_HANDLER_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_HANDLER_DEF_GUARD_
#include "common.h"

namespace mol {
namespace whiskey {

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

extern HttpHandler* processor();


class HttpInterceptor : public HttpHandler
{
public:
    HttpInterceptor() {}
    HttpInterceptor(const std::string& m, const std::string& p);
};

template<class T>
class GenericHandler : public HttpHandler
{
public:
    GenericHandler( const std::string& m, const std::string& p, T t ) 
        : HttpHandler(m,p), t_(t)
    {}    
    
    int request_handler( Request& req, Response& res ) 
    {    
        return t_(req,res);
    }
    
private:
    T t_;
};


class HandlerBuilder
{
public:
    HandlerBuilder()
    {
    }
    
    HandlerBuilder(const std::string& p)
        : path_(p)
    {}
    
    
    HandlerBuilder& path(const std::string& p)
    {
        path_ = p;
        return *this;
    }    
    
    HandlerBuilder& method(const std::string& m)
    {
        method_ = m;
        return *this;
    }  
       
    template<class T>
    GenericHandler<T> handle(T t)
    {
        return GenericHandler<T>(method_,path_,t);
    }
    
    template<class T>
    GenericHandler<T> operator()(T t)
    {
        return handle(t);
    }
        
protected:
    std::string method_;
    std::string path_;
};

class Get : public HandlerBuilder
{
public:
    Get(const std::string& p) : HandlerBuilder(p)
    {
        method("GET");
    }    
};

class Post : public HandlerBuilder
{
public:
    Post(const std::string& p) : HandlerBuilder(p)
    {
        method("POST");
    }    
};

class Put : public HandlerBuilder
{
public:
    Put(const std::string& p) : HandlerBuilder(p)
    {
        method("PUT");
    }    
};

class Delete : public HandlerBuilder
{
public:
    Delete(const std::string& p) : HandlerBuilder(p)
    {
        method("DELETE");
    }    
};



template<class T>
class GenericInterceptor : public HttpInterceptor
{
public:
    GenericInterceptor( const std::string& m, const std::string& p, T t ) 
        : HttpInterceptor(m,p), t_(t)
    {}    
    
    int request_handler( Request& req, Response& res ) 
    {    
        return t_(req,res);
    }
    
private:
    T t_;
};

template<class T>
GenericInterceptor<T> interceptor(const std::string& m, const std::string& p, T t)
{
    return GenericInterceptor<T>(m,p,t);
}

} // end namespace whiskey
} // end namespace mol

#endif

