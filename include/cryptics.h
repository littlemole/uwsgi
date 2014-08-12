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


class SymCrypt
{
public:
    SymCrypt( const EVP_CIPHER* cipher, const std::string& key);
    SymCrypt( const EVP_CIPHER* cipher, const std::string& key, const std::string& iv);
    ~SymCrypt();

    std::string encrypt(const std::string& input);    
    std::string decrypt (const std::string& raw);
    
    std::string iv();
    
    SymCrypt(const SymCrypt& d) = delete;
    SymCrypt& operator=(const SymCrypt& rhs) = delete;
    
private:
    const EVP_CIPHER* cipher_;
    EVP_CIPHER_CTX ctx_;
    std::string iv_;
    std::string key_;
};

class Envelope
{
public:
    Envelope(const EVP_CIPHER* cipher);
    Envelope(const EVP_CIPHER* cipher, const std::string& key, const std::string& iv);    
    ~Envelope();
    
    std::string key();
    std::string iv();

    std::string seal(EVP_PKEY* pubkey, const std::string& msg);
    std::string open(EVP_PKEY* privkey, const std::string & msg);

    Envelope(const Envelope& d) = delete;
    Envelope& operator=(const Envelope& rhs) = delete;
    
private:
    const EVP_CIPHER* cipher_;    
    EVP_CIPHER_CTX ctx_;
    
    unsigned char* key_;    
    int            ekl_;
    unsigned char* iv_;    
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
    
    std::string toDER();
    void fromDER(int type, const std::string& s);

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
    
    std::string toDER();
    void fromDER(int type, const std::string& s);
    
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

