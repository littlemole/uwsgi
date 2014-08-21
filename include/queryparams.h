#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_PARAMS_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_PARAMS_DEF_GUARD_
#include "common.h"

namespace mol {
namespace whiskey {


class QueryParams
{
public:

    QueryParams(const std::string& s);

    std::string get(const std::string& key);
        
private:
    std::map<std::string,std::string> params_;
};

} // end namespace whiskey
} // end namespace mol

#endif

