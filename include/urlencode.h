#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_URLENCODE_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_URLENCODE_DEF_GUARD_
#include "common.h"

namespace mol {
namespace whiskey {


class Urlencode
{
public:

    static std::string decode(const std::string& s);
    static std::string decode(const char* s, int len);
    static std::string encode(const std::string& s);
    static std::string encode(const char* s, int len);
};

} // end namespace whiskey
} // end namespace mol

#endif

