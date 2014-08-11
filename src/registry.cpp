
#include "registry.h"
#include <dirent.h> 
#include <stdio.h> 
#include "processor.h"
#include "response.h"

typedef int (*handler_t)(Processor* proc);


Registry::~Registry()
{
    for( auto it = modules_.begin(); it != modules_.end(); it++)
    {
        dlclose( (void*) (*it) );
    }
}

int Registry::load()
{
    uwsgi_log("Registry::load()\n");    
    
    std::vector<std::string> sos = glob(ucpp.modules_dir);
    for ( size_t i = 0; i < sos.size(); i++)
    {
        uwsgi_log("Registry::load() %s\n", sos[i].c_str());    
        if ( sos[i].find(".so") != std::string::npos )
        {
            std::ostringstream oss;
            oss << ucpp.modules_dir << "/" << sos[i];   
            std::string modulename = oss.str();    
                    
            void* handle = dlopen(modulename.c_str(), RTLD_LAZY);
            
            if (handle) 
            {
                modules_.push_back((size_t*)handle);
            }            
        }
    }
    return 1;
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
    
    Request  req(wsgi_req);    
    Response res(wsgi_req);
    
    HttpHandler* proc = processor();
    return proc->request_handler(req,res);
}


