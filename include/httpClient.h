#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_CLIENT_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_CLIENT_DEF_GUARD_
#include "common.h"
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime> 
#include <cmath>
#include <cstring>
#include <curl/curl.h>

// super simple curl exception class.
class CurlEx {};

// curl wrapper. 
class Curl
{
public:
    Curl();
    Curl(const std::string& u);
    ~Curl();
        
    Curl& url(const std::string& u);
    Curl& data( const std::string& formdata );
    Curl& method( const std::string& m );
    Curl& header(const std::string& key, const std::string& val);
    Curl& verbose();
    Curl& perform();
    
    long status();
    std::string response_body();
    headers_t& response_headers();
    std::string response_header( const std::string& key );
    
    void reset();
    
    Curl( const Curl& rhs ) = delete;
    Curl& operator=(const Curl& rhs) = delete;
private:

    static int callback(char *data, size_t size, size_t nmemb, Curl* that);
    static size_t rcvHeaders(void *buffer, size_t size, size_t nmemb, void *userp);

    void cleanup();
    void init();

    CURL* curl_;
    struct curl_slist* headers_;
    headers_t response_headers_;
    std::string m_;
    std::ostringstream oss_;
};

#endif


