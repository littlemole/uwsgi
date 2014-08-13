#include "cryptics.h"
#include "base64.h"

template<class T>
class Buffer
{
public:

    Buffer(size_t s)
        : buf_(s,0)
    {}
    
    T* operator&()
    {
        return &(buf_[0]);
    }
    
    T& operator[](size_t i)
    {
        return buf_[i];
    }
    
    std::string toString()
    {
        return std::string( (char*)&(buf_[0]), buf_.size()*sizeof(T) );
    }
    
private:
    std::vector<T> buf_;

};

typedef Buffer<unsigned char> uchar_buf;


static const char nibble_decode(char nibble)
{
    const char byte_map[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    return byte_map[(int)nibble];
}

std::string toHex(const std::string& input)
{
    unsigned char* bytes = (unsigned char*)input.c_str();

    std::ostringstream oss;
    for(size_t i = 0; i < input.size(); i++)
    {
        char c1 = nibble_decode(bytes[i] >> 4);
        char c2 = nibble_decode(bytes[i] & 0x0f);
        oss.write(&c1,1);
        oss.write(&c2,1);
    }
    return oss.str();
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
    std::ostringstream oss;
    
    const char * p = hex.c_str();
    while (*p)
    {
        char i = HexByteToInt((char*)p);
        oss.write(&i,1);
        p+=2;
    }        
    
    return oss.str();  
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
    uchar_buf key(n);
	int fd;

	if ((fd = open ("/dev/random", O_RDONLY)) == -1)
		perror ("open error");

	if ((read (fd, &key, n)) == -1)
		perror ("read key error");

    return key.toString();
}


SymCrypt::SymCrypt( const EVP_CIPHER* cipher, const std::string& key, const std::string& iv)
    : cipher_( cipher ), iv_(iv), key_(key)
{
}

SymCrypt::SymCrypt( const EVP_CIPHER* cipher, const std::string& key)
    : cipher_( cipher ), iv_(""), key_(key)
{
    int s = EVP_CIPHER_iv_length(cipher_);
    if ( s )
    {
        iv_ = nonce(s);
    }
}

SymCrypt::~SymCrypt()
{
}

std::string SymCrypt::iv()
{
    return iv_;
}


std::string SymCrypt::encrypt(const std::string& input)
{
    EVP_CIPHER_CTX_init(&ctx_);
    EVP_EncryptInit(
        &ctx_, 
        cipher_, 
        (unsigned char*)key_.c_str(), 
        (unsigned char*)iv_.c_str() 
    );
    
    int n = EVP_CIPHER_block_size(cipher_)+input.size()-1;
    
    uchar_buf outbuf(n+1);
    
    if(EVP_EncryptUpdate (&ctx_,&outbuf, &n, (unsigned char*)input.c_str(), input.size() ) != 1)
    {
        return "";
    }

    int tlen=0;
    if (EVP_EncryptFinal (&ctx_, (&outbuf) + n, &tlen) != 1)
    {
        return "";
    }
    n+=tlen;

    EVP_CIPHER_CTX_cleanup(&ctx_);    
    return std::string( (char*)&outbuf, n );
}

std::string SymCrypt::decrypt (const std::string& raw)
{
    EVP_CIPHER_CTX_init(&ctx_);
    EVP_DecryptInit(
        &ctx_, 
        cipher_, 
        (unsigned char*)key_.c_str(), 
        (unsigned char*)iv_.c_str() 
    );
    
    int n = EVP_CIPHER_block_size(cipher_)+raw.size();
    uchar_buf outbuf(n+1);

    if (EVP_DecryptUpdate (&ctx_, &outbuf, &n, (const unsigned char*)raw.c_str(), raw.size()) != 1)
    {
        return "";
    }

    int tlen = 0;
    if (EVP_DecryptFinal (&ctx_, &outbuf + n, &tlen) != 1)
    {
        return "";
    }
    n += tlen;
    
    EVP_CIPHER_CTX_cleanup(&ctx_);    
    return std::string( (char*)&outbuf, n );
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
    unsigned int len = EVP_MD_size(md_);

    uchar_buf buffer(len);

    if( HMAC_Update(&ctx_, (const unsigned char *) msg.c_str(), msg.size()) != 1)
    {
        return "";
    }
    
    if( HMAC_Final(&ctx_, &buffer, &len) != 1)
    {
        return "";
    }
    
    return buffer.toString();
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

std::string PrivateKey::toDER()
{
    int len = i2d_PrivateKey(pkey_, NULL);
    uchar_buf buf(len);

    unsigned char* p = &buf;
    i2d_PrivateKey(pkey_, &p);
    
    return buf.toString();
}

void PrivateKey::fromDER(int type, const std::string& k)
{
    const unsigned char* s = (const unsigned char*) k.c_str();
    pkey_ = d2i_PrivateKey(type,&pkey_, (const unsigned char **)&s, k.size() );
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

std::string PublicKey::toDER()
{
    int len = i2d_PUBKEY(pkey_, NULL);
    uchar_buf buf(len);

    unsigned char* p = &buf;    
    i2d_PUBKEY(pkey_, &p);
    
    return buf.toString();
}

void PublicKey::fromDER(int type, const std::string& k)
{
    const unsigned char* s = (const unsigned char*) k.c_str();
    d2i_PUBKEY(&pkey_, (const unsigned char **)&s, k.size() );
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
    
    uchar_buf sig(size);
    unsigned int len = 0;
    EVP_SignFinal(&ctx_,&sig,&len, pkey_ );        
    
    EVP_MD_CTX_cleanup(&ctx_); 
    return sig.toString();
}

bool Signature::verify(const std::string& msg,const std::string& sig)
{
    int r = EVP_VerifyInit(&ctx_,md_);
    
    r = EVP_VerifyUpdate(&ctx_, msg.c_str(), msg.size() );
    r = EVP_VerifyFinal( &ctx_,(unsigned char *)sig.c_str(), (unsigned int) sig.size(),pkey_);
    
    EVP_MD_CTX_cleanup(&ctx_); 
    return r == 1;
}    


Envelope::Envelope(const EVP_CIPHER* cipher)
    : cipher_(cipher)
{}

Envelope::Envelope(const EVP_CIPHER* cipher, const std::string& key, const std::string& iv)
    : cipher_(cipher)
{
    ekl_ = key.size();
    key_ = (unsigned char*) malloc(ekl_);
    memcpy( key_, key.c_str(), ekl_ );
        
    iv_  = (unsigned char*) malloc(iv.size());
    memcpy( iv_, iv.c_str(), iv.size() );
}

Envelope::~Envelope()
{
    free(key_);                  
    free(iv_);
}

std::string Envelope::key()
{
    return std::string( (char*)key_, ekl_ );
}

std::string Envelope::iv()
{
    return std::string( (char*)iv_, EVP_CIPHER_iv_length(cipher_) );
}

std::string Envelope::seal(EVP_PKEY* key, const std::string& msg)
{
    key_ = (unsigned char *)malloc( EVP_PKEY_size(key)  );
    iv_  = (unsigned char *)malloc(EVP_CIPHER_iv_length(cipher_));        
    ekl_ = 1;
    
    EVP_SealInit(&ctx_, cipher_, &key_, &ekl_, iv_, &key, 1);
              
    int n = EVP_CIPHER_block_size(cipher_)+msg.size()-1;
    uchar_buf outbuf(n+1);
                  
    if(EVP_SealUpdate (&ctx_, &outbuf, &n, (unsigned char*)msg.c_str(), msg.size() ) != 1)
    {
        return "";
    }

    int tlen=0;
    if (EVP_SealFinal (&ctx_, &outbuf + n, &tlen) != 1)
    {
        return "";
    }                  
    n+=tlen;

    EVP_CIPHER_CTX_cleanup(&ctx_);    
    return std::string( (char*)&outbuf, n );   
}

std::string Envelope::open(EVP_PKEY* key, const std::string & msg)
{
    EVP_OpenInit(&ctx_, cipher_, key_, ekl_, iv_, key);
              
    int n = EVP_CIPHER_block_size(cipher_)+msg.size()-1;
    uchar_buf outbuf(n+1);
                  
    if(EVP_OpenUpdate (&ctx_, &outbuf, &n, (unsigned char*)msg.c_str(), msg.size() ) != 1)
    {
        return "";
    }

    int tlen=0;
    if (EVP_OpenFinal (&ctx_, &outbuf + n, &tlen) != 1)
    {
        return "";
    }                  
    n+=tlen;

    EVP_CIPHER_CTX_cleanup(&ctx_);    
    return std::string( (char*)&outbuf, n  );
}



