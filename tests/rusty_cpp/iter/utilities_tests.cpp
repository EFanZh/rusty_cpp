#include <gtest/gtest.h>
#include <rusty_cpp/iter/utilities.h>

using rusty_cpp::iter::utilities::IsGoodMemberV;

TEST(IterUtilities, HasArrowOperator) {
    ASSERT_FALSE(IsGoodMemberV<int &&>);
    ASSERT_FALSE(IsGoodMemberV<const int>);
    ASSERT_FALSE(IsGoodMemberV<const int &&>);
    ASSERT_FALSE(IsGoodMemberV<void>);
    ASSERT_FALSE(IsGoodMemberV<void()>);
    ASSERT_FALSE(IsGoodMemberV<int[]>);

    ASSERT_TRUE(IsGoodMemberV<int>);
    ASSERT_TRUE(IsGoodMemberV<int &>);
    ASSERT_TRUE(IsGoodMemberV<const int &>);
    ASSERT_TRUE(IsGoodMemberV<int[7]>);
    ASSERT_TRUE(IsGoodMemberV<void (*)()>);
    ASSERT_TRUE(IsGoodMemberV<void (&)()>);
}
