#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_ARG_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_ARG_DEF_GUARD_
#include "common.h"

namespace mol {
namespace whiskey {


class Args
{
public:

    Args( const patharguments_t& args);

    std::string get(const std::string& key);
    std::vector<std::string> keys();
    
private:
    patharguments_t args_; 
};


} // end namespace whiskey
} // end namespace mol

#endif

