#include "gtest/gtest.h"

TEST(HelloTest, GetGreet) {
  EXPECT_EQ("Bazel", "Bazel");
}