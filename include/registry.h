#ifndef DEFINE_MOL_UWSGI_REGISTRY_DEFINE_
#define DEFINE_MOL_UWSGI_REGISTRY_DEFINE_

#include "common.h"

extern "C" {

    struct uwsgi_cpp 
    {
        char *modules_dir;
    };  

    extern struct uwsgi_cpp ucpp;
    
}

class Registry
{
public:
    Registry() {};
    ~Registry();

    int load();

    Registry(const Registry& rhs) = delete;
    Registry& operator=(const Registry& rhs) = delete;
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

