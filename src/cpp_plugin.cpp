
#include "registry.h"


struct uwsgi_cpp  ucpp;  
    
struct uwsgi_option uwsgi_cpp_options[] = 
{
    {"cpp-dir", required_argument, 0, "directory for modules", uwsgi_opt_set_str, &ucpp.modules_dir, 0 },
    { 0, 0, 0, 0, 0, 0, 0 }
};

static int uwsgi_cpp_init()
{
    uwsgi_log("CPP MODULE DIR : %s\n", ucpp.modules_dir);    
    int loader = registry().load();    
    uwsgi_log("LOADED : %i\n", loader);    
    return 0;
}

static int uwsgi_cpp_request(struct wsgi_request *wsgi_req) 
{
    return uwsgiProcessor::process(wsgi_req);
}

static void uwsgi_cpp_after_request(struct wsgi_request *wsgi_req) 
{
    uwsgi_log("after request\n");
}


struct uwsgi_plugin cpp_plugin = 
{
        .name = "cpp",
        .alias = 0,
        .modifier1 = 250,
        .data = 0,
        .on_load = 0,
        .init = uwsgi_cpp_init,
        .post_init = 0,
        .post_fork = 0,
        .options = uwsgi_cpp_options,
        .enable_threads = 0,
        .init_thread = 0,
        .request = uwsgi_cpp_request,
        .after_request = uwsgi_cpp_after_request

};


