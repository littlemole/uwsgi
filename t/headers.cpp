#include "request.h"
#include "processor.h"
#include "gtest/gtest.h"
#include "uwsgi.h"

extern std::map<std::string,std::string> wsgi_env_mock;
extern std::string wsgi_body_mock;

namespace {


class HeadersTest : public ::testing::Test {
 protected:


  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
   
}; // end test setup


TEST_F(HeadersTest, simpleRequest) {

    wsgi_env_mock.clear();
    wsgi_env_mock["REQUEST_METHOD"] = "GET";
    wsgi_env_mock["PATH_INFO"] = "/context/test";
    wsgi_env_mock["QUERY_STRING"] = "a=b&c=d";    

    wsgi_request r;
    
    Request req(&r);
    
    EXPECT_EQ("GET",req.method());
    EXPECT_EQ("/context/test",req.path());
    EXPECT_EQ("a=b&c=d",req.querystring());
    EXPECT_EQ("b",req.queryParams().get("a"));
    EXPECT_EQ("d",req.queryParams().get("c"));        
}


TEST_F(HeadersTest, simplePostRequest) {

    wsgi_env_mock.clear();
    wsgi_env_mock["REQUEST_METHOD"] = "POST";
    wsgi_env_mock["PATH_INFO"] = "/context/test";
    wsgi_env_mock["QUERY_STRING"] = "a=b&c=d";    
    wsgi_env_mock["CONTENT_TYPE"] = "text/plain"; 
        
    wsgi_body_mock = "a=b&c=d";
    
    wsgi_request r;
    
    Request req(&r);
    
    EXPECT_EQ("POST",req.method());
    EXPECT_EQ("/context/test",req.path());
    EXPECT_EQ("a=b&c=d",req.querystring());
    EXPECT_EQ("text/plain",req.content_type());
    EXPECT_EQ("a=b&c=d",req.body());
    
    QueryParams qp(req.body());
    EXPECT_EQ("b",qp.get("a"));
    EXPECT_EQ("d",qp.get("c"));
}

TEST_F(HeadersTest, simpleHandlerInfoTest) {
    
    HandlerInfo info("GET","/context/{arg}",0);

    patharguments_t result;
    bool b = info.match("GET","/context/test",result);           
    
    EXPECT_EQ(true,b);
    EXPECT_EQ(result[0].first,"arg");
    EXPECT_EQ(result[0].second,"test");
}

TEST_F(HeadersTest, simpleInterceptorInfoTest) {
    
    InterceptorInfo info("GET","/context/test.*",0);

    bool b = info.match("GET","/context/test/blabla");           
    
    EXPECT_EQ(true,b);
}


}  // namespace


