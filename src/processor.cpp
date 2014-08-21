#include "response.h"
#include "processor.h"
#include <iostream>
#include <memory>
#include "uwsgi.h"
    
    
namespace mol {
namespace whiskey {

    
HandlerInfo::HandlerInfo( const std::string& m,  const std::string& p, HttpHandler* handler ) 
{
    method_ = m;
    handler_ = handler;
    
    boost::regex r("\\{([^\\}]*)\\}");
    boost::smatch match;

    std::string::const_iterator start = p.begin();
    std::string::const_iterator end   = p.end();    
    
    while (boost::regex_search (start,end,match,r)) 
    {
        if ( match.size() > 1 )
        {
            args_.push_back(match[1]);
        }
        start = match[0].second;
    }
    std::string replacement("([^/]*)");
    std::string s = boost::regex_replace (p,r,replacement);
    path_regex_ = boost::regex(s);
    
}

bool HandlerInfo::match(const std::string& m, const std::string& p, patharguments_t& result) 
{
    if ( m != method_ ) 
    {        
        return false;
    }
    
    boost::smatch match;
    if ( !boost::regex_match(p,match,path_regex_) )
    {
        return false;
    }
        
    patharguments_t args;
    
    for ( size_t i = 0; i < args_.size(); i++)
    {
        args.push_back( pathargument_t(args_[i],match[i+1]) );
    }
    
    result = args;
    return true;
}    

int HandlerInfo::invoke(Request& req, Response& res) 
{
    return handler_->request_handler( req, res );
}

InterceptorInfo::InterceptorInfo( const std::string& m,  const std::string& p, HttpHandler* handler ) 
{
    method_ = m;
    handler_ = handler;
    path_regex_ = boost::regex(p);
    
}

bool InterceptorInfo::match(const std::string& m, const std::string& p) 
{
    if ( m != method_ ) 
    {        
        return false;
    }
    
    boost::smatch match;
    if ( !boost::regex_match(p,match,path_regex_) )
    {
        return false;
    }
    return true;
}    

int InterceptorInfo::invoke(Request& req, Response& res) 
{
    return handler_->request_handler( req, res );
}

Processor::Processor() 
{}

Processor& Processor::registerHandler( const std::string& method, const std::string& path, HttpHandler* handler) 
{
    uwsgi_log("Processor::registerHandler %s, %s\r\n",method.c_str(),path.c_str());
    handlers_.push_back( HandlerInfo(method,path,handler) );    
    return *this;
}


Processor& Processor::registerInterceptor( const std::string& method, const std::string& path, HttpHandler* handler) 
{
    uwsgi_log("Processor::registerInterceptor %s, %s\r\n",method.c_str(),path.c_str());
    interceptors_.push_back( InterceptorInfo(method,path,handler) );    
    return *this;
}

int Processor::intercept( Request& req, Response& res ) 
{
    std::string method = req.method();
    std::string path   = req.path();
    
    int r = UWSGI_OK;
    for ( size_t i = 0; i < interceptors_.size(); i++)
    {
        if ( interceptors_[i].match(method,path) )
        {
            r = interceptors_[i].invoke(req,res);
            if ( res.isDone() )
            {
                return r;
            }
        }
    }
    return r;
}

int Processor::request_handler( Request& req, Response& res ) 
{
    std::string method = req.method();
    std::string path   = req.path();
  
    //uwsgi_log("request_handler %s %s %i\n", method.c_str(), path.c_str(), handlers_.size()); 
    for ( size_t i = 0; i < handlers_.size(); i++) {
    
        //uwsgi_log("%s %s", method.c_str(), path.c_str() );
        patharguments_t args;
        if ( handlers_[i].match( method, path, args) ) {
        
            req.set_pathargs(args);
            
            int r = intercept(req,res);
            if ( res.isDone() )
            {
                res.flush();
                return r;
            }
            
            r = handlers_[i].invoke(req,res);
            res.flush();
            return r;
        }
    }
    
    return -1;
}


HttpHandler* processor()
{
    static Processor processor;
    return &processor;
}

HttpHandler::HttpHandler(const std::string& m, const std::string& p) 
{
    Processor* proc = (Processor*)processor();
    proc->registerHandler(m,p,this);             
}

HttpInterceptor::HttpInterceptor(const std::string& m, const std::string& p)
{
    Processor* proc = (Processor*)processor();
    proc->registerInterceptor(m,p,this);             
}

} // end namespace whiskey
} // end namespace mol



