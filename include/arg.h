#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_ARG_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_ARG_DEF_GUARD_
#include "common.h"


class Args
{
public:

    Args( const std::string& tpl, const std::string& path_info );

    std::string get(const std::string& key);
    
private:

    std::map<std::string,std::string> args_;
};


#endif

