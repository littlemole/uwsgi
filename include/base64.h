#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_BASE64_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_BASE64_DEF_GUARD_
#include "common.h"

class Base64
{
public:

    static std::string decode(const std::string& s);
    static std::string decode(const char* s);
    static std::string encode(const std::string& s, bool singleline=true);
    static std::string encode(const char* s, size_t len, bool singleline=true);
};

#endif

