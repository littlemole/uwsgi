#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_RESPONSE_JSON_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_RESPONSE_JSON_DEF_GUARD_

#include "jsoncpp/json/json.h"

namespace mol {
namespace whiskey {
namespace JSON {

class JsonParseEx {};

inline Json::Value parse(const std::string& txt)
{
    Json::Value json;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( txt, json );
    if(!parsingSuccessful)
    {
        throw JsonParseEx();
    }
    return json;
}

inline const std::string stringify(Json::Value value)
{
    Json::StyledWriter writer;
    return writer.write( value );    
}


} // end namespace JSON
} // end namespace whiskey
} // end namespace mol

#endif

