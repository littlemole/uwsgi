#include "common.h"
#include "gtest/gtest.h"
#include <sstream>
#include <vector>
#include <string>
#include "base64.h"
#include "cryptics.h"

 
// uwsgi mocks

extern std::map<std::string,std::string> wsgi_env_mock;
extern std::string wsgi_body_mock;

// uwsgi spies

extern std::string wsgi_status_spy;
extern headers_t wsgi_headers_spy;
extern std::string wsgi_body_spy;


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

TEST_F(BasicTest, SignTest) {

    OpenSSL_add_all_algorithms();
    
    std::string input = "a well known secret";
    PrivateKey privateKey("pem/private.pem");
    PublicKey publicKey("pem/public.pem");
    
    Signature signor( EVP_sha1(), privateKey );
    
    std::string sig = signor.sign(input);
    std::cerr << toHex(sig) << std::endl;
    
    Signature verifier( EVP_sha1(), publicKey );
    
    bool verified = verifier.verify(input,sig);    
    
    EXPECT_EQ(true,verified);
}

}  // namespace


