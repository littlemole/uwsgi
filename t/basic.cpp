#include "common.h"
#include "gtest/gtest.h"
#include <sstream>

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

}  // namespace


