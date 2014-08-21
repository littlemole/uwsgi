#include "jwt.h"
#include "cryptics.h"
#include "json.h"
#include "base64.h"

namespace mol {
namespace whiskey {

JWT::JWT()
{
}

JWT::~JWT()
{
}

bool JWT::verify(const std::string& token, const std::string& secret)
{
    std::vector<std::string> vjwt = split(token,'.');
    if( vjwt.size() != 3) {
        return false;
    }
    
    std::string jwt_header    = Base64Url::decode(vjwt[0]);
    std::string jwt_claim     = Base64Url::decode(vjwt[1]);
    
    signature_ = Base64Url::decode(vjwt[2]);
    
    try {
        header_ = JSON::parse( jwt_header );
        claim_ = JSON::parse( jwt_claim );
    } catch( JSON::JsonParseEx ex)
    {
        return false;
    }
    
    unsigned int exp = claim_.get("exp",0).asInt();
    unsigned int now = time(NULL);
    if ( now > exp )
    {
        return false;
    }
    
    size_t pos = token.find_last_of(".");
    if ( pos == std::string::npos ) {
        return false;    
    }
    
    Hmac hmac(EVP_sha256(),secret);
    std::string hash = hmac.hash(token.substr(0,pos));

    if ( hash == signature_ ) {
        return true;
    }    
    return false;
}

Json::Value JWT::header()
{
    return header_;
}

Json::Value JWT::claim()
{
    return claim_;
}

std::string JWT::signature()
{
    return Base64Url::encode(signature_);
}



} // end namespace whiskey
} // end namespace mol



