#include "base64.h"
#include <iostream>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <string.h>
#include <stdio.h>

namespace mol {
namespace whiskey {
 
//Calculates the length of a decoded base64 string
int calcDecodeLength(const char* b64input) 
{ 
    int len = strlen(b64input);
    int padding = 0;
     
    if (b64input[len-1] == '=' && b64input[len-2] == '=') 
        padding = 2;
    else if (b64input[len-1] == '=') 
        padding = 1;
     
    return (int)len*0.75 - padding;
}

std::string Base64::decode(const std::string& input)
{
    std::string s = input;           
    
    size_t missing_padding = s.size() % 3;
    if ( missing_padding != 0)
    {
        missing_padding = 3 - missing_padding;
    }
    for ( size_t i = 0; i < missing_padding; i++) {
        s += "=";
    }
                
    int decodeLen = calcDecodeLength(s.c_str());
                      
    char_buf buffer(decodeLen+1);
    FILE* stream = fmemopen((char*)s.c_str(), s.size(), "r");
     
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* bio = BIO_new_fp(stream, BIO_NOCLOSE);
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); 
    int len = BIO_read(bio, &buffer, s.size());
     
    BIO_free_all(bio);
    fclose(stream);
    return buffer.toString(len);
}

std::string Base64::decode(const char* s)
{
    return decode( std::string(s) );
}

std::string Base64::encode(const std::string& s, bool singleline)
{
    BIO *b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    BIO *mem = BIO_new(BIO_s_mem()); 
    BIO_push(b64, mem);
    
    bool done = false;
    int res = 0;
    while(!done)
    {
        res = BIO_write(b64, s.c_str(), s.size());

        if(res <= 0) 
        {
            if(BIO_should_retry(b64))
            {
                continue;
            }
            else
            {
                throw Base64ex();
            }
        }
        else
        {
            done = true;
        }
    }

    BIO_flush(b64);

    unsigned char* output;
    int len = BIO_get_mem_data(mem, &output);

    std::string result((char*)output, len);
    BIO_free(mem);
    BIO_free(b64);
    return result;    
}

std::string Base64::encode(const char* s, size_t len, bool singleline)
{
    return encode( std::string(s,len), singleline);
}


std::string Base64Url::decode(const std::string& input)
{
    std::ostringstream oss;
    
    for ( size_t i = 0; i < input.size(); i++)
    {
        switch( input[i] )
        {
            case '-' : 
            {
                oss << '+';
                break;
            }
            case '_' : 
            {
                oss << '/';
                break;
            }          
            default : 
            {
                oss << input[i];
            }
        }
    }

    return Base64::decode(oss.str());
}

std::string Base64Url::decode(const char* s)
{
    return decode( std::string(s) );
}

std::string Base64Url::encode(const std::string& s)
{
    std::string input = Base64::encode(s,true);
    
    std::ostringstream oss;
    for ( size_t i = 0; i < input.size(); i++)
    {
        switch( input[i] )
        {
            case '+' : 
            {
                oss << '-';
                break;
            }
            case '/' : 
            {
                oss << '_';
                break;
            }          
            case '=' : 
            {
                break;
            }                    
            default : 
            {
                oss << input[i];
            }
        }
    }
    
    return oss.str();
}

std::string Base64Url::encode(const char* s, size_t len)
{
    return encode( std::string(s,len));
}


} // end namespace whiskey
} // end namespace mol

