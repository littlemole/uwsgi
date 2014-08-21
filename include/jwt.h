#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_CRYPT_JWT_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_CRYPT_JWT_DEF_GUARD_
#include "common.h"
#include "json.h"

namespace mol {
namespace whiskey {

class JWT
{
public:
    JWT();
    ~JWT();

    bool verify(const std::string& token, const std::string& secret);
    
    Json::Value header();
    Json::Value claim();
    std::string signature();

private:
    Json::Value header_;
    Json::Value claim_;
    std::string signature_;
};

} // end namespace whiskey
} // end namespace mol

#endif

