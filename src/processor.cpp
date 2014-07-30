#include "common.h"

    
HandlerInfo::HandlerInfo( const std::string& m,  const std::string& p, HttpHandler* handler ) {
    method_ = m;
    path_ = split(p,'/');
    handler_ = handler;
}

bool HandlerInfo::match(const std::string& m, const std::string& p, patharguments_t& result) {

    std::cerr << m << " ?= " << method_ << std::endl;
    if ( m != method_ ) {
        
        return false;
    }
        
    patharguments_t args;
    std::vector<std::string> v = split(p,'/');
    std::cerr << path_.size() << " >? " << v.size() << std::endl;
    if ( path_.size() > v.size() ) {
        return false;
    }
    for ( size_t i = 0; i < path_.size(); i++) {
        std::cerr << path_[i] << " ?= " << v[i] << std::endl;
        if ( !path_[i].empty() && path_[i][0] == '{' ) {
            std::string key = path_[i];
            std::string value = v[i];
            args.push_back( pathargument_t(key,value) );
            continue;
        }
        if ( path_[i] != v[i] ) {
        }
    }
    result = args;
    return true;      
}    

int HandlerInfo::invoke(Request& req, Response& res) {
    return handler_->request_handler( req, res );
}


   

Processor::Processor() 
{}

Processor& Processor::registerHandler( const std::string& method, const std::string& path, HttpHandler* handler) {

    handlers_.push_back( HandlerInfo(method,path,handler) );    
    return *this;
}


int Processor::request_handler( Request& req, Response& res ) 
{
    std::string method = req.method();
    std::string path   = req.path();
  
    std::cerr << "request_handler" << method << " " << path << " " << handlers_.size() << std::endl;

    for ( size_t i = 0; i < handlers_.size(); i++) {
    
        std::cerr << method << " " << path << std::endl;
        patharguments_t args;
        if ( handlers_[i].match( method, path, args) ) {
            std::cerr << "MATCH" << std::endl;
            req.set_pathargs(args);
            return handlers_[i].invoke(req,res);
        }
    }
    
    return HTTP_BAD_REQUEST;
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
 


