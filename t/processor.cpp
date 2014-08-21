#include "request.h"
#include "response.h"
#include "processor.h"
#include "gtest/gtest.h"
#include "uwsgi.h"

using namespace mol::whiskey;

extern std::map<std::string,std::string> wsgi_env_mock;
extern std::string wsgi_body_mock;

extern std::string wsgi_status_spy;
extern headers_t wsgi_headers_spy;
extern std::string wsgi_body_spy;

namespace {

class Handler : public HttpHandler
{
public:

    int request_handler( Request& req, Response& res ) 
    {
        res << "TEST\n";
        return res.done().ok();
    }
};

static Handler handler;

class Interceptor : public HttpInterceptor
{
public:

    int request_handler( Request& req, Response& res ) 
    {
        return res.done().bad_request();
    };
};

static Interceptor myinterceptor;


class ProcessorTest : public ::testing::Test {
 protected:


  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
   
}; // end test setup


TEST_F(ProcessorTest, simpleRequest) {

    wsgi_env_mock.clear();
    wsgi_env_mock["REQUEST_METHOD"] = "GET";
    wsgi_env_mock["PATH_INFO"] = "/test";
    wsgi_env_mock["QUERY_STRING"] = "a=b&c=d";    

    wsgi_request r;
    
    Request req(&r);
    Response res(&r);
    
    Processor processor;
    processor.registerHandler("GET","/test",&handler);
    int result = processor.request_handler(req,res);
    
    EXPECT_EQ(0,result);    
    EXPECT_EQ(true,res.isDone());
    EXPECT_EQ("TEST\n",wsgi_body_spy);    
    EXPECT_EQ("200 OK",wsgi_status_spy);
}

TEST_F(ProcessorTest, simpleInterceptorRequest) {

    wsgi_env_mock.clear();
    wsgi_env_mock["REQUEST_METHOD"] = "GET";
    wsgi_env_mock["PATH_INFO"] = "/test/admin";
    wsgi_env_mock["QUERY_STRING"] = "a=b&c=d";    

    wsgi_request r;
    
    Request req(&r);
    Response res(&r);
    
    Processor processor;
    processor.registerHandler("GET","/test/admin",&handler);
    processor.registerInterceptor("GET","/test/.*",&myinterceptor);
    int result = processor.request_handler(req,res);
    
    EXPECT_EQ(0,result);    
    EXPECT_EQ(true,res.isDone());
    EXPECT_EQ("400 bad request",wsgi_status_spy);
}


}  // namespace


