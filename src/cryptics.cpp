#include "cryptics.h"
#include "base64.h"

static const char nibble_decode(char nibble)
{
    const char byte_map[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    return byte_map[(int)nibble];
}

std::string toHex(const std::string& input)
{
    unsigned char* bytes = (unsigned char*)input.c_str();
    int length = input.size();
    char* buffer = new char[length*2+1];
    for(int i = 0; i < length; i++){
        buffer[i*2] = nibble_decode(bytes[i] >> 4);
        buffer[i*2+1] = nibble_decode(bytes[i] & 0x0f);
       // buffer[i*3+2] = ' ';
    }
    buffer[length*2] = '\0';
    std::string result(buffer,length*2);
    delete[] buffer;
    return result;
}


static int HexCharToInt(char ch)
{
    if (ch >= '0' && ch <= '9')
        return (ch-'0');
    else if (ch >= 'a' && ch <= 'f')
        return (ch-'a'+10);
    else if (ch >= 'A' && ch <= 'F')
        return (ch-'A'+10);
    else
        throw std::invalid_argument("Not a valid hex digit.");
}


static int HexByteToInt(const char * hex)
{
    return HexCharToInt(hex[0]) * 16
        + HexCharToInt(hex[1]);
}


std::string fromHex(const std::string& hex)
{
    char* buf = new char[hex.size()/2+1];
    char* dest = buf;
    const char * p = hex.c_str();
    while (*p)
    {
        char i = HexByteToInt((char*)p);
        dest[0] = i;
        p+=2;
        dest+=1;        
    }    
    dest[0] = 0;
    
    std::string result((char*)buf,hex.size()/2);
    delete[] buf;
    return result;
}

Digest::Digest(const std::string& md)
{
    md_ = EVP_get_digestbyname(md.c_str());
    mdctx_ = EVP_MD_CTX_create();        
}

Digest::Digest(const EVP_MD * md)
{
    md_ = md;
    mdctx_ = EVP_MD_CTX_create();    
}

Digest::~Digest()
{
    EVP_MD_CTX_destroy(mdctx_);
}

std::string Digest::digest(const std::string& input)
{
     unsigned char md_value[EVP_MAX_MD_SIZE];
     unsigned int md_len = 0;;         

     EVP_DigestInit_ex(mdctx_, md_, NULL);
     EVP_DigestUpdate(mdctx_, input.c_str(), input.size());
     EVP_DigestFinal_ex(mdctx_, md_value, &md_len);
     
     return std::string( (char*) md_value, md_len );
}

std::string Digest::hex(const std::string& input)
{
    return toHex(digest(input));
}

std::string Digest::base64(const std::string& input)
{
    return Base64::encode(digest(input));
}    

std::string md5( const std::string& s)
{
    Digest evp(EVP_md5());
    return evp.digest(s);
}


std::string sha1( const std::string& s)
{
    Digest evp(EVP_sha1());
    return evp.digest(s);
}


std::string sha256( const std::string& s)
{
    Digest evp(EVP_sha256());
    return evp.digest(s);
}

std::string nonce (int n)
{
    unsigned char* key = new unsigned char[n];
	int fd;

	if ((fd = open ("/dev/random", O_RDONLY)) == -1)
		perror ("open error");

	if ((read (fd, key, n)) == -1)
		perror ("read key error");

    std::string result( (char*) key, n );
    delete[] key;
    return result;
}


Encrypt::Encrypt( const std::string& key, const std::string& iv)
    : cipher_( EVP_bf_cbc() ), iv_(iv), key_(key)
{
    EVP_CIPHER_CTX_init(&ctx_);
    EVP_EncryptInit(
        &ctx_, 
        cipher_, 
        (unsigned char*)key_.c_str(), 
        (unsigned char*)iv_.c_str() 
    );
}

Encrypt::~Encrypt()
{
    EVP_CIPHER_CTX_cleanup(&ctx_);
}

std::string Encrypt::encrypt(const std::string& input)
{
    int n = EVP_CIPHER_block_size(cipher_)+input.size()-1;
    unsigned char* outbuf = new unsigned char[n+1];
    
    if(EVP_EncryptUpdate (&ctx_, outbuf, &n, (unsigned char*)input.c_str(), input.size() ) != 1)
    {
        return "";
    }

    int tlen=0;
    if (EVP_EncryptFinal (&ctx_, outbuf + n, &tlen) != 1)
    {
        return "";
    }
    n+=tlen;
    std::string result( (char*)outbuf, n );
    delete[] outbuf;
    return result;
}


Decrypt::Decrypt( const std::string& key, const std::string& iv )
    : cipher_( EVP_bf_cbc() ), iv_(iv), key_(key)
{
    EVP_CIPHER_CTX_init(&ctx_);
    EVP_DecryptInit(
        &ctx_, 
        cipher_, 
        (unsigned char*)key_.c_str(), 
        (unsigned char*)iv_.c_str() 
    );
}

Decrypt::~Decrypt()
{
    EVP_CIPHER_CTX_cleanup(&ctx_);
}
std::string Decrypt::decrypt (const std::string& raw)
{
    int n = EVP_CIPHER_block_size(cipher_)+raw.size();
    unsigned char* outbuf = new unsigned char[n+1];

    if (EVP_DecryptUpdate (&ctx_, outbuf, &n, (const unsigned char*)raw.c_str(), raw.size()) != 1)
    {
        return "";
    }

    int tlen = 0;
    if (EVP_DecryptFinal (&ctx_, outbuf + n, &tlen) != 1)
    {
        return "";
    }
    n += tlen;
    
    std::string result( (char*)outbuf, n );
    delete[] outbuf;
    return result;        
}


Hmac::Hmac(const EVP_MD* md, const std::string& key)
    : md_(md), key_(key)
{
    HMAC_CTX_init(&ctx_);
    HMAC_Init(&ctx_, key.c_str(), key.size(), md_ );
}

Hmac::~Hmac()
{
    HMAC_CTX_cleanup(&ctx_);
}

std::string Hmac::hash(const std::string& msg)
{
    unsigned char* buffer = 0;
    unsigned int len = EVP_MD_size(md_);

    buffer = new unsigned char[len];

    if( HMAC_Update(&ctx_, (const unsigned char *) msg.c_str(), msg.size()) != 1)
    {
        return "";
    }
    
    if( HMAC_Final(&ctx_, buffer, &len) != 1)
    {
        return "";
    }
    
    std::string result((char*)buffer,len);
    delete[] buffer;
    return result;
}

PrivateKey::PrivateKey()
{
    pkey_ = EVP_PKEY_new();
}

PrivateKey::PrivateKey(const std::string& file)
{   
    FILE* f = fopen(file.c_str(),"r");
    if (!f) 
    {
        throw std::string("file not found");
    }
    pkey_ = PEM_read_PrivateKey(f, NULL, 0, 0);        
    fclose(f);
}

PrivateKey::~PrivateKey()
{
    EVP_PKEY_free(pkey_);
}


PublicKey::PublicKey()
{
    pkey_ = EVP_PKEY_new();
}

PublicKey::PublicKey(const std::string& file)
{ 
    FILE* f = fopen(file.c_str(),"r");
    if (!f) 
    {
        throw std::string("file not found");
    }
    pkey_ = PEM_read_PUBKEY(f, NULL, 0, 0);        
    fclose(f);            
} 

PublicKey::~PublicKey()
{
    EVP_PKEY_free(pkey_);
}

Signature::Signature(const EVP_MD* md,EVP_PKEY* key)
    : md_(md),pkey_(key)
{
    
}

std::string Signature::sign(const std::string& msg)
{
    EVP_SignInit(&ctx_, md_);
    int size = EVP_PKEY_size(pkey_);
    
    EVP_SignUpdate(&ctx_, msg.c_str(), msg.size() );
    
    unsigned char* sig = new unsigned char [size];
    unsigned int len = 0;
    EVP_SignFinal(&ctx_,sig,&len, pkey_ );        
    
    std::string result((char*)sig,len);
    
    delete[] sig;
    EVP_MD_CTX_cleanup(&ctx_); 
    return result;
}

bool Signature::verify(const std::string& msg,const std::string& sig)
{
    int r = EVP_VerifyInit(&ctx_,md_);
    
    r = EVP_VerifyUpdate(&ctx_, msg.c_str(), msg.size() );
    r = EVP_VerifyFinal( &ctx_,(unsigned char *)sig.c_str(), (unsigned int) sig.size(),pkey_);
    
    EVP_MD_CTX_cleanup(&ctx_); 
    return r == 1;
}    

