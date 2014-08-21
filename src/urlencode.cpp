#include "urlencode.h"
#include <iostream>
#include <curl/curl.h>

namespace mol {
namespace whiskey {


std::string Urlencode::decode(const std::string& s)
{
    return decode( s.c_str(), s.size() );
}

std::string Urlencode::decode(const char* s, int len)
{
    char* c = curl_unescape( s , len );
    std::string result(c);    
    curl_free(c);
    return result;
}

std::string Urlencode::encode(const std::string& s)
{
    return encode( s.c_str(), s.size() );
}

std::string Urlencode::encode(const char* s, int len)
{
    CURL* curl = curl_easy_init();
    char* c = curl_easy_escape( curl , s , len );
    std::string result(c);    
    curl_free(c);
    curl_easy_cleanup(curl);
    return result;    
}    

} // end namespace whiskey
} // end namespace mol

