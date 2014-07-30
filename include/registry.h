#ifndef DEFINE_MOL_UWSGI_REGISTRY_DEFINE_
#define DEFINE_MOL_UWSGI_REGISTRY_DEFINE_

#include "response.h"

class Processor;

extern "C" {

    struct uwsgi_cpp 
    {
        char *modules_dir;
    };  

    extern struct uwsgi_server uwsgi;
    extern struct uwsgi_cpp ucpp;
    
   // typedef int(*handler_t)(Request& req, Response& res);  
   
}

class Registry
{
public:

    ~Registry();

    int load();

private:
    std::vector<size_t*> modules_;
};

Registry& registry();


class uwsgiProcessor
{
public:

    static int process(wsgi_request *wsgi_req);
};

#endif

