#include <gtest/gtest.h>
#include <rusty_cpp/iter/iterable.h>
#include <set>
#include <vector>

using rusty_cpp::iter::iterable::Iterable;
using rusty_cpp::iter::iterable::make_iterable;
using std::as_const;
using std::begin;
using std::declval;
using std::end;
using std::is_same_v;
using std::make_tuple;
using std::remove_cvref_t;
using std::set;
using std::size;
using std::vector;

namespace {
template <class C>
struct CheckContainer {
    // Precondition.

    static_assert(is_same_v<C, remove_cvref_t<C>>);

    // Type definitions.

    using Iterator = decltype(begin(declval<C &>()));
    using ConstIterator = decltype(begin(declval<const C &>()));
    using SizeType = decltype(size(declval<const C &>()));

    // Check `make_iterable`.

    static_assert(is_same_v<decltype(make_iterable(declval<C &>())), Iterable<C &>>);
    static_assert(is_same_v<decltype(make_iterable(declval<const C &>())), Iterable<const C &>>);
    static_assert(is_same_v<decltype(make_iterable(declval<C &&>())), Iterable<C>>);

    // Check `Iterable::begin()`.

    static_assert(is_same_v<decltype(begin(declval<Iterable<C>>())), ConstIterator>);
    static_assert(is_same_v<decltype(begin(declval<Iterable<C &>>())), Iterator>);
    static_assert(is_same_v<decltype(begin(declval<Iterable<const C &>>())), ConstIterator>);

    static_assert(is_same_v<decltype(begin(declval<Iterable<C> &>())), Iterator>);
    static_assert(is_same_v<decltype(begin(declval<Iterable<C &> &>())), Iterator>);
    static_assert(is_same_v<decltype(begin(declval<Iterable<const C &> &>())), ConstIterator>);

    static_assert(is_same_v<decltype(begin(declval<const Iterable<C>>())), ConstIterator>);
    static_assert(is_same_v<decltype(begin(declval<const Iterable<C &>>())), Iterator>);
    static_assert(is_same_v<decltype(begin(declval<const Iterable<const C &>>())), ConstIterator>);

    static_assert(is_same_v<decltype(begin(declval<const Iterable<C> &>())), ConstIterator>);
    static_assert(is_same_v<decltype(begin(declval<const Iterable<C &> &>())), Iterator>);
    static_assert(is_same_v<decltype(begin(declval<const Iterable<const C &> &>())), ConstIterator>);

    // Check `Iterable::end()`.

    static_assert(is_same_v<decltype(end(declval<Iterable<C>>())), ConstIterator>);
    static_assert(is_same_v<decltype(end(declval<Iterable<C &>>())), Iterator>);
    static_assert(is_same_v<decltype(end(declval<Iterable<const C &>>())), ConstIterator>);

    static_assert(is_same_v<decltype(end(declval<Iterable<C> &>())), Iterator>);
    static_assert(is_same_v<decltype(end(declval<Iterable<C &> &>())), Iterator>);
    static_assert(is_same_v<decltype(end(declval<Iterable<const C &> &>())), ConstIterator>);

    static_assert(is_same_v<decltype(end(declval<const Iterable<C>>())), ConstIterator>);
    static_assert(is_same_v<decltype(end(declval<const Iterable<C &>>())), Iterator>);
    static_assert(is_same_v<decltype(end(declval<const Iterable<const C &>>())), ConstIterator>);

    static_assert(is_same_v<decltype(end(declval<const Iterable<C> &>())), ConstIterator>);
    static_assert(is_same_v<decltype(end(declval<const Iterable<C &> &>())), Iterator>);
    static_assert(is_same_v<decltype(end(declval<const Iterable<const C &> &>())), ConstIterator>);

    // Check `Iterable::size()`.

    static_assert(is_same_v<decltype(size(declval<const Iterable<C> &>())), SizeType>);
    static_assert(is_same_v<decltype(size(declval<const Iterable<C &> &>())), SizeType>);
    static_assert(is_same_v<decltype(size(declval<const Iterable<const C &> &>())), SizeType>);

    // Result.

    static constexpr auto result = true;
};
} // namespace

TEST(IterIterable, StaticChecks) {
    static_assert(CheckContainer<vector<int>>::result);
    static_assert(CheckContainer<set<double>>::result);
    static_assert(CheckContainer<float[7]>::result);
}

TEST(IterIterable, OwnedBeginEnd) {
    auto iterable = make_iterable(vector<int>{2, 3, 5, 7, 11});

    const auto begin_1 = iterable.begin();
    const auto begin_2 = as_const(iterable).begin();
    const auto begin_3 = iterable.cbegin();
    const auto begin_4 = as_const(iterable).begin();

    ASSERT_EQ(begin_1, begin_2);
    ASSERT_EQ(begin_1, begin_3);
    ASSERT_EQ(begin_1, begin_4);

    ASSERT_EQ(begin_1[0], 2);
    ASSERT_EQ(begin_1[1], 3);
    ASSERT_EQ(begin_1[2], 5);
    ASSERT_EQ(begin_1[3], 7);
    ASSERT_EQ(begin_1[4], 11);

    const auto end_1 = iterable.end();
    const auto end_2 = as_const(iterable).end();
    const auto end_3 = iterable.cend();
    const auto end_4 = as_const(iterable).cend();

    ASSERT_EQ(end_1, end_2);
    ASSERT_EQ(end_1, end_3);
    ASSERT_EQ(end_1, end_4);

    ASSERT_EQ(end_1 - begin_1, 5);
}

TEST(IterIterable, OwnedSize) {
    auto iterable = make_iterable(vector<int>{2, 3, 5, 7, 11});

    ASSERT_EQ(iterable.size(), size_t{5});
}

TEST(IterIterable, NotOwnedBeginEnd) {
    auto data = vector<int>{2, 3, 5, 7, 11};
    auto iterable = make_iterable(data);

    const auto begin_1 = iterable.begin();
    const auto begin_2 = as_const(iterable).begin();
    const auto begin_3 = iterable.cbegin();
    const auto begin_4 = as_const(iterable).begin();

    ASSERT_EQ(begin_1, begin_2);
    ASSERT_EQ(begin_1, begin_3);
    ASSERT_EQ(begin_1, begin_4);

    ASSERT_EQ(begin_1[0], 2);
    ASSERT_EQ(begin_1[1], 3);
    ASSERT_EQ(begin_1[2], 5);
    ASSERT_EQ(begin_1[3], 7);
    ASSERT_EQ(begin_1[4], 11);

    const auto end_1 = iterable.end();
    const auto end_2 = as_const(iterable).end();
    const auto end_3 = iterable.cend();
    const auto end_4 = as_const(iterable).cend();

    ASSERT_EQ(end_1, end_2);
    ASSERT_EQ(end_1, end_3);
    ASSERT_EQ(end_1, end_4);

    ASSERT_EQ(end_1 - begin_1, 5);
}

TEST(IterIterable, NotOwnedSize) {
    auto data = vector<int>{2, 3, 5, 7, 11};
    auto iterable = make_iterable(data);

    ASSERT_EQ(iterable.size(), size_t{5});
}

TEST(IterIterable, Collect) {
    auto data = vector<int>{2, 3, 5, 7, 11};
    auto expected = set<int>{2, 3, 5, 7, 11};
    auto collected_1 = make_iterable(data).collect<set<int>>();
    auto collected_2 = make_iterable(vector<int>{2, 3, 5, 7, 11}).collect<set<int>>();

    ASSERT_EQ(collected_1, expected);
    ASSERT_EQ(collected_2, expected);
}
