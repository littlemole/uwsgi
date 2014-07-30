#ifndef DEFINE_MOL_UWSGI_REGISTRY_DEFINE_
#define DEFINE_MOL_UWSGI_REGISTRY_DEFINE_

#include "response.h"

extern "C" {

    struct uwsgi_cpp 
    {
        char *modules_dir;
    };  

    extern struct uwsgi_server uwsgi;
    extern struct uwsgi_cpp ucpp;
    
    typedef int(*handler_t)(Request& req, Response& res);  
}

class Registry
{
public:

    ~Registry();

    handler_t get(const std::string& name);

private:
    std::vector<size_t*> modules_;
    std::map<std::string,handler_t> handlers_;
};

Registry& registry();


class uwsgiProcessor
{
public:

    static int process(wsgi_request *wsgi_req);
};

#endif

