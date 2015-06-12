#include <gtest/gtest.h>
#include "allowedtypes.hpp"

using allowed::AllowedTypes;

TEST(AllowedTypes, Empty)
{
  AllowedTypes obj;
  EXPECT_TRUE(obj.empty());
}

TEST(AllowedTypes, NonEmpty)
{
  AllowedTypes obj;
  obj.append("one");
  obj.append("two");
  obj.append("three");

  EXPECT_FALSE(obj.empty());

  EXPECT_TRUE(obj.has("one"));
  EXPECT_FALSE(obj.has("four"));
}
