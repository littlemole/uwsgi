#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_CRYPT_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_REQUEST_CRYPT_DEF_GUARD_
#include "common.h"
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/pem.h>
#include <openssl/err.h>

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
    
    Digest(const Digest& d) = delete;
    Digest& operator=(const Digest& rhs) = delete;
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
    
    Encrypt(const Encrypt& d) = delete;
    Encrypt& operator=(const Encrypt& rhs) = delete;
    
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

    Decrypt(const Decrypt& d) = delete;
    Decrypt& operator=(const Decrypt& rhs) = delete;

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
    
    Hmac(const Hmac& d) = delete;
    Hmac& operator=(const Hmac& rhs) = delete;

private:
    const EVP_MD* md_;
    std::string key_;
    HMAC_CTX ctx_;
};

class PrivateKey
{
public:
    PrivateKey();
    PrivateKey(const std::string& file);
    
    ~PrivateKey();
    
    operator EVP_PKEY* ()
    {
        return pkey_;
    }

    PrivateKey(const PrivateKey& d) = delete;
    PrivateKey& operator=(const PrivateKey& rhs) = delete;

private:
    EVP_PKEY* pkey_;
};

class PublicKey
{
public:
    PublicKey();
    PublicKey(const std::string& file);
    ~PublicKey();
    
    operator EVP_PKEY* ()
    {
        return pkey_;
    }
    
    PublicKey(const PublicKey& d) = delete;
    PublicKey& operator=(const PublicKey& rhs) = delete;    

private:
    EVP_PKEY* pkey_;    
};

class Signature
{
public:
    Signature(const EVP_MD* md,EVP_PKEY* key);

    std::string sign(const std::string& msg);    
    bool verify(const std::string& msg,const std::string& sig);

    Signature(const Signature& d) = delete;
    Signature& operator=(const Signature& rhs) = delete;

private:
    const EVP_MD* md_;
    EVP_PKEY* pkey_;
    EVP_MD_CTX ctx_;
};


#endif

