#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_ARG_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_ARG_DEF_GUARD_
#include "common.h"

typedef std::pair<std::string,std::string> pathargument_t;
typedef std::vector<pathargument_t> patharguments_t;


class Args
{
public:

    Args( const patharguments_t& args);

    std::string get(const std::string& key);
    std::vector<std::string> keys();
    
private:
    patharguments_t args_; 
};


#endif

