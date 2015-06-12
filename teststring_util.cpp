#include <gtest/gtest.h>
#include <stdint.h>
#include "string_util.hpp"

TEST(string_util, Zero)
{
  EXPECT_EQ(toStr(static_cast<int16_t>(0.)), "0");
  EXPECT_EQ(toStr(static_cast<uint16_t>(0.)), "0");
  EXPECT_EQ(toStr(static_cast<int32_t>(0.)), "0");
  EXPECT_EQ(toStr(static_cast<uint32_t>(0.)), "0");
  EXPECT_EQ(toStr(static_cast<int64_t>(0.)), "0");
  EXPECT_EQ(toStr(static_cast<uint64_t>(0.)), "0");
  EXPECT_EQ(toStr(static_cast<float>(0.)), "0");
  EXPECT_EQ(toStr(static_cast<double>(0.)), "0");
}

TEST(string_util, One)
{
  EXPECT_EQ(toStr(static_cast<int16_t>(1.)), "1");
  EXPECT_EQ(toStr(static_cast<uint16_t>(1.)), "1");
  EXPECT_EQ(toStr(static_cast<int32_t>(1.)), "1");
  EXPECT_EQ(toStr(static_cast<uint32_t>(1.)), "1");
  EXPECT_EQ(toStr(static_cast<int64_t>(1.)), "1");
  EXPECT_EQ(toStr(static_cast<uint64_t>(1.)), "1");
  EXPECT_EQ(toStr(static_cast<float>(1.)), "1");
  EXPECT_EQ(toStr(static_cast<double>(1.)), "1");
}

TEST(string_util, Point_Five)
{
  EXPECT_EQ(toStr(static_cast<float>(.5)), "0.5");
  EXPECT_EQ(toStr(static_cast<double>(.5)), "0.5");
}
