
#include "registry.h"
#include <dirent.h> 
#include <stdio.h> 
#include "processor.h"

typedef int (*handler_t)(Processor* proc);

std::vector<std::string> glob(const std::string& f)
{
  DIR           *d;
  struct dirent *dir;
  
  std::vector<std::string> result;
  
  uwsgi_log("glob dir %s\n", f.c_str()); 
  d = opendir(f.c_str());
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
        uwsgi_log("glob %s\n", dir->d_name); 
        if (dir->d_type == DT_REG)
        {   
            std::string n = std::string(dir->d_name);
            if ( n != "." && n != "..")
            {
                result.push_back(dir->d_name);
            }
        }
    }
    closedir(d);
  }
  return result;
}

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


