
#include "registry.h"

Registry::~Registry()
{
    for( auto it = modules_.begin(); it != modules_.end(); it++)
    {
        dlclose( (void*) (*it) );
    }
}

handler_t Registry::get(const std::string& name)
{
    uwsgi_log("get handler: %s\n", name.c_str());    
    
    if ( handlers_.count(name) == 0 )
    {
        handlers_[name] = 0;
        
        std::ostringstream oss;
        oss << ucpp.modules_dir << "/" << name << "_mod.so";
        std::string soname = oss.str();
        
        uwsgi_log("load .so: %s\n", soname.c_str());
        
        void* handle = dlopen(soname.c_str(), RTLD_LAZY);
        
        if (handle) 
        {
            modules_.push_back((size_t*)handle);
            
            dlerror();
            
            std::ostringstream oss;
            oss << name << "_request_handler";   
            std::string handlername = oss.str();        
            
            uwsgi_log("load handler: %s\n", handlername.c_str()); 
            
            handler_t h = (handler_t) dlsym(handle,handlername.c_str());            
            const char *dlsym_error = dlerror();
            if (!dlsym_error && h) 
            {
                 uwsgi_log("successfully loaded handler: %s\n", handlername.c_str()); 
                 handlers_[name] = h;
            }
        }
    }
    
    handler_t handler = handlers_[name];
    return handler;
}    


Registry& registry()
{
    static Registry registry;
    return registry;
}

int uwsgiProcessor::process(wsgi_request *wsgi_req)
{
	if (uwsgi_parse_vars(wsgi_req)) 
	{
        return -1;
    }
    
    uint16_t len = 0;
    char* path_info = uwsgi_get_var(wsgi_req, (char *) "PATH_INFO", 9, &len);
    if (!path_info)
    {
        return -1;
    }

    std::string ctx(path_info,len);
    while (!ctx.empty() && ctx[0] == '/' ) {
        ctx = ctx.substr(1);
    }
    
    while (!ctx.empty() && (ctx[ctx.size()-1] == '\n' || ctx[ctx.size()-1] == '\r') ) {
        ctx = ctx.substr(0,ctx.size()-1);
    }    
    
    size_t pos = ctx.find("/");
    if ( pos != std::string::npos )
    {
        ctx = ctx.substr(0,pos);
    }
    
    uwsgi_log("handler: %s\n", ctx.c_str());    
    
    handler_t handler = registry().get(ctx);
    if (handler)
    {
        Request  req(wsgi_req);
        Response res(wsgi_req);
        int r = handler(req,res);
        return r;    
    }
    return -1;
}


