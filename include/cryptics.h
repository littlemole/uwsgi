#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_CRYPT_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_CRYPT_DEF_GUARD_
#include "common.h"
#include <openssl/evp.h>
#include <openssl/hmac.h>

std::string nonce (int n);

std::string toHex(const std::string& input);
std::string fromHex(const std::string& hex);

class Digest
{
public:
    Digest(const std::string& md);
    Digest(const EVP_MD * md);
    ~Digest();
    
    std::string digest(const std::string& input);
    std::string hex(const std::string& input);
    std::string base64(const std::string& input);
    
private:
    EVP_MD_CTX* mdctx_;
    const EVP_MD* md_;
};

std::string md5( const std::string& s);
std::string sha1( const std::string& s);
std::string sha256( const std::string& s);


class Encrypt
{
public:

    Encrypt( const std::string& key, const std::string& iv);
    ~Encrypt();

    std::string encrypt(const std::string& input);    
    
private:
    const EVP_CIPHER* cipher_;
    EVP_CIPHER_CTX ctx_;
    std::string iv_;
    std::string key_;
};

class Decrypt
{
public:

    Decrypt( const std::string& key, const std::string& iv );
    ~Decrypt();

    std::string decrypt (const std::string& raw);

private:
    const EVP_CIPHER* cipher_;
    EVP_CIPHER_CTX ctx_;
    std::string iv_;
    std::string key_;
};

class Hmac
{
public:
    Hmac(const EVP_MD* md, const std::string& key);
    ~Hmac();
    
    std::string hash(const std::string& msg);
    
private:
    const EVP_MD* md_;
    std::string key_;
    HMAC_CTX ctx_;
};


#endif

