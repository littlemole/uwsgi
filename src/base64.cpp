#include "base64.h"
#include <iostream>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <string.h>
#include <stdio.h>
 
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

std::string Base64::decode(const std::string& s)
{
    int decodeLen = calcDecodeLength(s.c_str());
    char* buffer = (char*)malloc(decodeLen+1);
    FILE* stream = fmemopen((char*)s.c_str(), s.size(), "r");
     
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* bio = BIO_new_fp(stream, BIO_NOCLOSE);
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); 
    int len = BIO_read(bio, buffer, s.size());
    buffer[len] = '\0';
     
    BIO_free_all(bio);
    fclose(stream);
    std::string result(buffer,len);
    free(buffer);
    return result;
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

        if(res <= 0) // if failed
        {
            if(BIO_should_retry(b64))
            {
                continue;
            }
            else // encoding failed
            {
                return "";
            }
        }
        else // success!
        {
            done = true;
        }
    }

    BIO_flush(b64);

    // get a pointer to mem's data
    unsigned char* output;
    int len = BIO_get_mem_data(mem, &output);

    // assign data to output
    std::string result((char*)output, len);
    BIO_free(mem);
    BIO_free(b64);
    return result;    

/*
    const char* message = s.c_str();
    char* buffer = 0;
    
    BIO *bio, *b64;
    FILE* stream;
    int encodedSize = 4*ceil((double)strlen(message)/3);
    buffer = (char *)malloc(encodedSize+1);
     
    stream = fmemopen(buffer, encodedSize+1, "w");
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_fp(stream, BIO_NOCLOSE);
    bio = BIO_push(b64, bio);
    if (singleline)
        BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); 
    BIO_write(bio, message, strlen(message));
    BIO_flush(bio);
    BIO_free_all(bio);
    fclose(stream);
    std::string result(buffer,encodedSize);
    free(buffer);
    return result;
    */
}

std::string Base64::encode(const char* s, size_t len, bool singleline)
{
    return encode( std::string(s,len),singleline );
}    

/*
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

*/

