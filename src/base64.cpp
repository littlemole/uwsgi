#include "base64.h"
#include <iostream>
#include "b64/encode.h"
#include "b64/decode.h"

std::string Base64::decode(const std::string& s)
{
    base64::decoder E;
    std::istringstream iss(s);
    std::ostringstream oss;
    E.decode(iss,oss);

    return oss.str();
}

std::string Base64::decode(const char* s)
{
    return decode( std::string(s) );
}

std::string Base64::encode(const std::string& s, bool singleline)
{
    base64::encoder E;
    std::istringstream iss(s);
    std::ostringstream oss;
    E.encode(iss,oss);

    std::string result = oss.str();    

    if ( singleline )
    {
        std::ostringstream oss;
        for( char c : result )
        {
            if ( c != '\n' )
            {
                oss.write(&c,1);    
            }
        }
        result = oss.str();
    }
    else
    {
        if ( !result.empty() && result[result.size()-1] == '\n' )
        {
            result = result.substr(0,result.size()-1);
        }    
    }
    return result;
}

std::string Base64::encode(const char* s, size_t len, bool singleline)
{
    return encode( std::string(s,len),singleline );
}    


