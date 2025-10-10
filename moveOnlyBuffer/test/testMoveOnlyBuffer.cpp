#include <gtest/gtest.h>
#include "moveOnlyBuffer.hpp"

TEST(MoveOnlyBuffer, Basic) {
  CMoveOnlyBuffer b(256);
  EXPECT_EQ(b.getVal(), 256);
  EXPECT_NE(b.getDataPtr(), nullptr);
}

TEST(MoveOnlyBuffer, MoveSemantics) {
  CMoveOnlyBuffer a(128);
  auto p = a.getDataPtr();
  CMoveOnlyBuffer b = std::move(a);
  EXPECT_EQ(b.getVal(), 128);
  EXPECT_EQ(b.getDataPtr(), p);
  EXPECT_EQ(a.getDataPtr(), nullptr);
}
