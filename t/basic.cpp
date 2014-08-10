#include "common.h"
#include "gtest/gtest.h"
#include <sstream>
#include <vector>
#include <string>
#include "base64.h"
#include "cryptics.h"

// uwsgi mocks

std::map<std::string,std::string> wsgi_mock;
std::string wsgi_body_mock;

// uwsgi spies

std::string wsgi_status_spy;
headers_t wsgi_headers_spy;
std::string wsgi_body_spy;

extern "C" {

void uwsgi_log(const char* c, ...)
{
}

int uwsgi_parse_vars(wsgi_request* r)
{
    return 0;
}

char* uwsgi_request_body_read( wsgi_request* r, ssize_t s,ssize_t* len)
{
    *len = wsgi_body_mock.size();
    return (char*)wsgi_body_mock.c_str();
}

char* uwsgi_get_var(wsgi_request* r, char * s, uint16_t size,uint16_t* len)
{
    *len = 0;
    std::string key(s,size);

    if ( wsgi_mock.count(key) == 0 )
    {
        return 0;
    }    
    
    const std::string& val = wsgi_mock[key];
    *len = val.size();
    
    return (char*)val.c_str();
}

int uwsgi_response_prepare_headers(wsgi_request* r,char* s ,uint16_t size)
{
    wsgi_status_spy = std::string(s,size);
    return 0;
}

int uwsgi_response_add_header(wsgi_request* r,char* k ,uint16_t ks,char* v ,uint16_t vs)
{
    std::string key = std::string(k,ks);
    std::string val = std::string(v,vs);
    
    wsgi_headers_spy.push_back( header_t(key,val) );
    return 0;
}

int uwsgi_response_write_body_do(wsgi_request* r,char* s ,size_t size)
{
    wsgi_body_spy.append( std::string(s,size) );
    return 0;
}

void uwsgi_opt_set_str(char* c, char* s, void* v)
{
}

}

int uwsgi_websocket_handshake(struct wsgi_request *, char *, uint16_t, char *, uint16_t)
{
    return 0;
}

int uwsgi_websocket_send(struct wsgi_request *, char *, size_t)
{
return 0;
}

struct uwsgi_buffer *uwsgi_websocket_recv(struct wsgi_request *)
{
return 0;
}

void uwsgi_buffer_destroy(uwsgi_buffer* b)
{}



namespace {


class BasicTest : public ::testing::Test {
 protected:


  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
   
}; // end test setup


TEST_F(BasicTest, TestClassesCompile) {

    std::string input = "/bla/blub/wupp/";
    std::vector<std::string> v = split(input,'/');
    EXPECT_EQ(3,v.size());
    EXPECT_EQ("bla",v[0]);
    EXPECT_EQ("blub",v[1]);
    EXPECT_EQ("wupp",v[2]);
}


TEST_F(BasicTest, Base64Test) {

    std::string input = "/bla/blub/wupp/bla/blub/wupp/bla/blub/wupp/bla/blub/wupp/";
    std::string b64 = Base64::encode(input);
    EXPECT_EQ("L2JsYS9ibHViL3d1cHAvYmxhL2JsdWIvd3VwcC9ibGEvYmx1Yi93dXBwL2JsYS9ibHViL3d1cHAv",b64);
    
}


TEST_F(BasicTest, Base64decodeTest) {

    std::string input = "L2JsYS9ibHViL3d1cHAvYmxhL2JsdWIvd3VwcC9ibGEvYmx1Yi93dXBwLw==";
    std::string plain = Base64::decode(input);
    EXPECT_EQ("/bla/blub/wupp/bla/blub/wupp/bla/blub/wupp/",plain);
    
}


TEST_F(BasicTest, MD5Test) {

    std::string input = "a well known secret";
    std::string hash = toHex(md5(input));
    EXPECT_EQ("e981fe735ca6982848f913eb0d9d254d",hash);

}


TEST_F(BasicTest, evpTest) {

    OpenSSL_add_all_digests();
    std::string input = "a well known secret";
    Digest evp("md5");
    std::string hash = toHex(evp.digest(input));
    EXPECT_EQ("e981fe735ca6982848f913eb0d9d254d",hash);

    hash = toHex(evp.digest(input));
    EXPECT_EQ("e981fe735ca6982848f913eb0d9d254d",hash);
}


TEST_F(BasicTest, hextest2) {

    unsigned char hex[] = { 1, 244, 27, 0, 4, 5, 0 };
    std::string s( (char*)hex, 6 );
    std::string hexed = toHex( s );
    EXPECT_EQ("01f41b000405",hexed);
    
    std::string raw = fromHex(hexed);
    EXPECT_EQ(s,raw);
    
}


TEST_F(BasicTest, sha1test) {

    std::string input = "a well known secret";
    Digest evp("sha1");
    std::string hash = toHex(evp.digest(input));
    EXPECT_EQ("652e0dbf69408801392353ba386313bf01ff04ce",hash);

    hash = toHex(evp.digest(input));
    EXPECT_EQ("652e0dbf69408801392353ba386313bf01ff04ce",hash);
}


TEST_F(BasicTest, sha1test2) {

    std::string input = "a well known secret";
    Digest evp(EVP_sha1());
    std::string hash = toHex(evp.digest(input));
    EXPECT_EQ("652e0dbf69408801392353ba386313bf01ff04ce",hash);

    hash = toHex(evp.digest(input));
    EXPECT_EQ("652e0dbf69408801392353ba386313bf01ff04ce",hash);
}


TEST_F(BasicTest, sha1test3) {

    std::string input = "a well known secret";
    std::string hash = toHex(sha1(input));
    EXPECT_EQ("652e0dbf69408801392353ba386313bf01ff04ce",hash);

    hash = toHex(sha1(input));
    EXPECT_EQ("652e0dbf69408801392353ba386313bf01ff04ce",hash);
}



TEST_F(BasicTest, sha256test) {

    std::string input = "a well known secret";
    std::string hash = toHex(sha256(input));
    EXPECT_EQ("428b79463ec0b5b89379da202f663116f93cbdb99632a86cf84183bbf787c2af",hash);

    hash = toHex(sha256(input));
    EXPECT_EQ("428b79463ec0b5b89379da202f663116f93cbdb99632a86cf84183bbf787c2af",hash);
}



TEST_F(BasicTest, bfTest) {

    std::string input = "a well known secret";
    std::string key   = "the secret secret key";
    std::string iv    = nonce(8);
    
    Encrypt encrypt(key,iv);
    std::string cipher = encrypt.encrypt(input);
    
    std::cerr << toHex(cipher) << std::endl;
    std::cerr << toHex(iv) << std::endl;

    Decrypt decrypt(key,iv);
    std::string plain = decrypt.decrypt(cipher);

    EXPECT_EQ(input,plain);
    
}


TEST_F(BasicTest, hmacMD5Test) {

    std::string input = "a well known secret";
    std::string key   = "the secret secret key";
    
    Hmac hmac(EVP_md5(),key);
    
    std::string hash = hmac.hash(input);
    std::cerr << toHex(hash) << std::endl;

    Hmac hmac2(EVP_md5(),key);
    std::string input2 = "a well known secret";
    std::string hash2 = hmac2.hash(input2);
    std::cerr << toHex(hash2) << std::endl;
    
    EXPECT_EQ(hash,hash2);
    
}

TEST_F(BasicTest, hmacSha1Test) {

    std::string input = "a well known secret";
    std::string key   = "the secret secret key";
    
    Hmac hmac(EVP_sha1(),key);
    
    std::string hash = hmac.hash(input);
    std::cerr << toHex(hash) << std::endl;

    Hmac hmac2(EVP_sha1(),key);
    std::string input2 = "a well known secret";
    std::string hash2 = hmac2.hash(input2);
    std::cerr << toHex(hash2) << std::endl;
    
    EXPECT_EQ(hash,hash2);
    
}

}  // namespace


