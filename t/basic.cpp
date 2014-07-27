#include "registry.h"
//#include "urlencode.h"
#include "gtest/gtest.h"


namespace {


class BasicTest : public ::testing::Test {
 protected:


  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
   
}; // end test setup


TEST_F(BasicTest, TestClassesCompile) {

    EXPECT_EQ("OK","OK");
}

}  // namespace


