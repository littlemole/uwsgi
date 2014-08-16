#include "httpClient.h"


Curl::Curl()
    : curl_(NULL), headers_(NULL)
{
    init();
}

Curl::Curl(const std::string& u)
    : curl_(NULL), headers_(NULL)
{
    init();
    url(u);
}    

Curl::~Curl()
{
    cleanup();
}
    
Curl& Curl::url(const std::string& u)
{
    curl_easy_setopt(curl_, CURLOPT_URL, u.c_str() );
    return *this;
}

Curl& Curl::data( const std::string& formdata ) {
   
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, formdata.c_str() );
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDSIZE, formdata.size() );
    return *this;
}

Curl& Curl::method( const std::string& m ) 
{
    m_ = m;
    curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, m_.c_str() );
    return *this;
}    
  
Curl& Curl::header(const std::string& key, const std::string& val)
{
    std::ostringstream oss;
    oss << key << ":" << val;
    headers_ = curl_slist_append(headers_, oss.str().c_str() );
    return *this;
}

Curl& Curl::verbose()
{
    curl_easy_setopt(curl_, CURLOPT_VERBOSE, 1L);
    return *this;
}

Curl& Curl::perform()
{
    CURLcode res = CURLE_OK;
    if(headers_ != NULL)
    {
        curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers_);
    }
    
    res = curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, &Curl::callback);
    if (res != CURLE_OK)
    {
        throw CurlEx();
    }
     
    res = curl_easy_setopt(curl_, CURLOPT_WRITEDATA, this);
    if (res != CURLE_OK)
    {
        throw CurlEx();
    }
    
    res = curl_easy_setopt(curl_, CURLOPT_HEADERFUNCTION, &Curl::rcvHeaders);          
    if (res != CURLE_OK)
    {
        throw CurlEx();
    }
    
    res = curl_easy_setopt(curl_, CURLOPT_WRITEHEADER, this);
    if (res != CURLE_OK)
    {
        throw CurlEx();
    }
    

    res = curl_easy_perform(curl_);
    if (res != CURLE_OK)
    {
        throw CurlEx();
    }
    return *this;
}

long Curl::status() 
{
    long http_code = 0;
    curl_easy_getinfo (curl_, CURLINFO_RESPONSE_CODE, &http_code);
    return http_code;
}

std::string Curl::response_body()
{
    return oss_.str();
}

headers_t& Curl::response_headers()
{
    return response_headers_;
}

std::string Curl::response_header( const std::string& key )
{
    for ( size_t i = 0; i < response_headers().size(); i++ )
    {
        if ( strcasecmp( response_headers()[i].first.c_str(), key.c_str()) == 0 )
        {
            return response_headers()[i].second;
        }
    }   
    return "";     
}

void Curl::reset()
{
    init();
}


void Curl::cleanup()
{
    if ( curl_ != NULL )
    {
        curl_easy_cleanup(curl_);
        if(headers_ != NULL)
        {
            curl_slist_free_all(headers_ );        
        }    
        
    }
    curl_ = NULL;
    headers_ = NULL;
    
    response_headers_.clear();
    m_ = "";
    oss_.str("");
    oss_.clear();
}
    
void Curl::init()
{
    cleanup();
    curl_ = curl_easy_init();
    if(!curl_) 
    {
        throw CurlEx();
    }
}


int Curl::callback(char *data, size_t size, size_t nmemb, Curl* that)
{
    that->oss_ << std::string(data,size*nmemb);
    return size * nmemb;
}

size_t Curl::rcvHeaders(void *buffer, size_t size, size_t nmemb, void *userp) 
{
    Curl* curl = (Curl*)(userp);
 
    size_t len = size * nmemb;
    std::string s = std::string( (char*)buffer, len );
    size_t pos = s.find(':');
    if ( pos != std::string::npos )
    {
        std::string key = s.substr(0,pos);
        std::string val = s.substr(pos+1,s.size()-pos-3);
        curl->response_headers_.push_back( header_t(key,val) );
    }
    
    return len;      
}


