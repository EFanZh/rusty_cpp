#include <gtest/gtest.h>
#include <rusty_cpp/iter/iterable.h>
#include <vector>

using rusty_cpp::iter::iterable::make_iterable;
using std::size_t;
using std::tuple;
using std::vector;

TEST(IterEnumerate, Default) {
    auto result = make_iterable(vector<int>{2, 3, 5, 7, 11}).enumerate().collect<vector<tuple<size_t, int>>>();
    vector<tuple<size_t, int>> expected_result = {{size_t{0}, 2}, {size_t{1}, 3}, {size_t{2}, 5}, {size_t{3}, 7}, {size_t{4}, 11}};

    ASSERT_EQ(result, expected_result);
}

TEST(IterEnumerate, Customized) {
    auto result = make_iterable(vector<int>{2, 3, 5, 7, 11}).enumerate<int>(7).collect<vector<tuple<int, int>>>();
    vector<tuple<int, int>> expected_result = {{7, 2}, {8, 3}, {9, 5}, {10, 7}, {11, 11}};

    ASSERT_EQ(result, expected_result);
}
