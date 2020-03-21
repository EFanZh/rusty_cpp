#include <gtest/gtest.h>
#include <rusty_cpp/iter/iterable.h>
#include <set>
#include <vector>

using rusty_cpp::iter::iterable::Iterable;
using rusty_cpp::iter::iterable::make_iterable;
using rusty_cpp::iter::map::Map;
using std::as_const;
using std::back_inserter;
using std::begin;
using std::copy;
using std::declval;
using std::end;
using std::is_same_v;
using std::make_pair;
using std::pair;
using std::remove_cvref_t;
using std::set;
using std::size;
using std::vector;

namespace {
template <class C>
struct CheckContainer {
    // Precondition.

    static_assert(is_same_v<C, remove_cvref_t<C>>);

    // Check `Iterable::map()`.

    struct F;

    static_assert(is_same_v<decltype(declval<Iterable<C>>().map(declval<F>())), Iterable<Map<C, F>>>);
    static_assert(is_same_v<decltype(declval<Iterable<C>>().map(declval<F &>())), Iterable<Map<C, F &>>>);
    static_assert(is_same_v<decltype(declval<Iterable<C>>().map(declval<const F &>())), Iterable<Map<C, const F &>>>);

    static_assert(is_same_v<decltype(declval<Iterable<C &>>().map(declval<F>())), Iterable<Map<C &, F>>>);
    static_assert(is_same_v<decltype(declval<Iterable<C &>>().map(declval<F &>())), Iterable<Map<C &, F &>>>);
    static_assert(is_same_v<decltype(declval<Iterable<C &>>().map(declval<const F &>())), Iterable<Map<C &, const F &>>>);

    static_assert(is_same_v<decltype(declval<Iterable<const C &>>().map(declval<F>())), Iterable<Map<const C &, F>>>);
    static_assert(is_same_v<decltype(declval<Iterable<const C &>>().map(declval<F &>())), Iterable<Map<const C &, F &>>>);
    static_assert(is_same_v<decltype(declval<Iterable<const C &>>().map(declval<const F &>())), Iterable<Map<const C &, const F &>>>);

    //

    static_assert(is_same_v<decltype(declval<Iterable<C> &>().map(declval<F>())), Iterable<Map<C &, F>>>);
    static_assert(is_same_v<decltype(declval<Iterable<C> &>().map(declval<F &>())), Iterable<Map<C &, F &>>>);
    static_assert(is_same_v<decltype(declval<Iterable<C> &>().map(declval<const F &>())), Iterable<Map<C &, const F &>>>);

    static_assert(is_same_v<decltype(declval<Iterable<C &> &>().map(declval<F>())), Iterable<Map<C &, F>>>);
    static_assert(is_same_v<decltype(declval<Iterable<C &> &>().map(declval<F &>())), Iterable<Map<C &, F &>>>);
    static_assert(is_same_v<decltype(declval<Iterable<C &> &>().map(declval<const F &>())), Iterable<Map<C &, const F &>>>);

    static_assert(is_same_v<decltype(declval<Iterable<const C &> &>().map(declval<F>())), Iterable<Map<const C &, F>>>);
    static_assert(is_same_v<decltype(declval<Iterable<const C &> &>().map(declval<F &>())), Iterable<Map<const C &, F &>>>);
    static_assert(is_same_v<decltype(declval<Iterable<const C &> &>().map(declval<const F &>())), Iterable<Map<const C &, const F &>>>);

    //

    static_assert(is_same_v<decltype(declval<const Iterable<C> &>().map(declval<F>())), Iterable<Map<const C &, F>>>);
    static_assert(is_same_v<decltype(declval<const Iterable<C> &>().map(declval<F &>())), Iterable<Map<const C &, F &>>>);
    static_assert(is_same_v<decltype(declval<const Iterable<C> &>().map(declval<const F &>())), Iterable<Map<const C &, const F &>>>);

    static_assert(is_same_v<decltype(declval<const Iterable<C &> &>().map(declval<F>())), Iterable<Map<C &, F>>>);
    static_assert(is_same_v<decltype(declval<const Iterable<C &> &>().map(declval<F &>())), Iterable<Map<C &, F &>>>);
    static_assert(is_same_v<decltype(declval<const Iterable<C &> &>().map(declval<const F &>())), Iterable<Map<C &, const F &>>>);

    static_assert(is_same_v<decltype(declval<const Iterable<const C &> &>().map(declval<F>())), Iterable<Map<const C &, F>>>);
    static_assert(is_same_v<decltype(declval<const Iterable<const C &> &>().map(declval<F &>())), Iterable<Map<const C &, F &>>>);
    static_assert(is_same_v<decltype(declval<const Iterable<const C &> &>().map(declval<const F &>())), Iterable<Map<const C &, const F &>>>);

    // Result.

    static constexpr auto result = true;
};

template <class T>
auto clone(const T &value) {
    return value;
}

template <class C, class F, class V>
void run_map_tests(C c, F f, V &&visitor) {
    auto borrowed_iterable = make_iterable(c);

    visitor(borrowed_iterable.map(f));
    visitor(borrowed_iterable.map(as_const(f)));
    visitor(borrowed_iterable.map(clone(f)));

    visitor(as_const(borrowed_iterable).map(f));
    visitor(as_const(borrowed_iterable).map(as_const(f)));
    visitor(as_const(borrowed_iterable).map(clone(f)));

    visitor(clone(borrowed_iterable).map(f));
    visitor(clone(borrowed_iterable).map(as_const(f)));
    visitor(clone(borrowed_iterable).map(clone(f)));

    auto const_borrowed_iterable = make_iterable(as_const(c));

    visitor(const_borrowed_iterable.map(f));
    visitor(const_borrowed_iterable.map(as_const(f)));
    visitor(const_borrowed_iterable.map(clone(f)));

    visitor(as_const(const_borrowed_iterable).map(f));
    visitor(as_const(const_borrowed_iterable).map(as_const(f)));
    visitor(as_const(const_borrowed_iterable).map(clone(f)));

    visitor(clone(const_borrowed_iterable).map(f));
    visitor(clone(const_borrowed_iterable).map(as_const(f)));
    visitor(clone(const_borrowed_iterable).map(clone(f)));

    auto owned_iterable = make_iterable(clone(c));

    visitor(owned_iterable.map(f));
    visitor(owned_iterable.map(as_const(f)));
    visitor(owned_iterable.map(clone(f)));

    visitor(as_const(owned_iterable).map(f));
    visitor(as_const(owned_iterable).map(as_const(f)));
    visitor(as_const(owned_iterable).map(clone(f)));

    visitor(clone(owned_iterable).map(f));
    visitor(clone(owned_iterable).map(as_const(f)));
    visitor(clone(owned_iterable).map(clone(f)));
}
} // namespace

TEST(IterMap, StaticChecks) {
    static_assert(CheckContainer<vector<int>>::result);
    static_assert(CheckContainer<set<double>>::result);
    static_assert(CheckContainer<float[7]>::result);
}

TEST(IterMap, RangeBasedLoop) {
    run_map_tests(
        vector<int>{2, 3, 5, 7, 11},
        [](auto x) { return x - 1; },
        [](auto mapped) {
            vector<int> result_container;
            vector<int> expected_result{1, 2, 4, 6, 10};

            for (auto x : mapped) {
                result_container.push_back(x);
            }

            ASSERT_EQ(result_container, expected_result);
        });

    run_map_tests(
        set<int>{2, 3, 5, 7, 11},
        [](auto x) { return x - 1; },
        [](auto mapped) {
            set<int> result_container;
            set<int> expected_result{1, 2, 4, 6, 10};

            for (auto x : mapped) {
                result_container.insert(x);
            }

            ASSERT_EQ(result_container, expected_result);
        });
}

TEST(IterMap, Copy) {
    run_map_tests(
        vector<int>{2, 3, 5, 7, 11},
        [](auto x) { return x - 1; },
        [](auto mapped) {
            vector<int> result_container;
            vector<int> expected_result{1, 2, 4, 6, 10};

            copy(mapped.begin(), mapped.end(), back_inserter(result_container));

            ASSERT_EQ(result_container, expected_result);
        });

    run_map_tests(
        set<int>{2, 3, 5, 7, 11},
        [](auto x) { return x - 1; },
        [](auto mapped) {
            vector<int> result_container;
            vector<int> expected_result{1, 2, 4, 6, 10};

            copy(mapped.begin(), mapped.end(), back_inserter(result_container));

            ASSERT_EQ(result_container, expected_result);
        });
}

TEST(IterMap, ReferencedValue) {
    vector<int> source{2, 3, 5, 7, 11};
    vector<int> expected_result{1, 2, 4, 6, 10};

    for (auto &x : make_iterable(source).map([](int &x) -> int & { return x; })) {
        x -= 1;
    }

    ASSERT_EQ(source, expected_result);
}

TEST(IterMap, Pointer) {
    vector<pair<int, int>> source{make_pair(2, 3), make_pair(5, 7), make_pair(11, 13)};
    vector<pair<int, int>> expected_result{make_pair(1, 1), make_pair(4, 3), make_pair(10, 6)};

    auto mapped = make_iterable(source).map([](auto &x) -> decltype(auto) { return x; });

    for (auto it = mapped.begin(); it != mapped.end(); ++it) {
        it->first -= 1;
        it->second /= 2;
    }

    ASSERT_EQ(source, expected_result);
}

TEST(IterMap, Subscript) {
    vector<pair<int, int>> source{make_pair(2, 3), make_pair(5, 7), make_pair(11, 13)};
    vector<pair<int, int>> expected_result{make_pair(1, 1), make_pair(4, 3), make_pair(10, 6)};

    auto mapped = make_iterable(source).map([](auto &x) -> decltype(auto) { return x; });
    auto it = mapped.begin();

    for (auto i = size_t{0}; i != mapped.size(); ++i) {
        it[i].first -= 1;
        it[i].second /= 2;
    }

    ASSERT_EQ(source, expected_result);
}

TEST(IterMap, PartialReference) {
    vector<pair<int, int>> source{make_pair(2, 3), make_pair(5, 7), make_pair(11, 13)};
    vector<pair<int, int>> expected_result{make_pair(1, 3), make_pair(4, 7), make_pair(10, 13)};

    for (auto &x : make_iterable(source).map([](auto &x) -> int & { return x.first; })) {
        x -= 1;
    }

    ASSERT_EQ(source, expected_result);
}

TEST(IterMap, VectorOfBool) {
    vector<bool> source{false, true, false, false, true};
    vector<bool> expected_result{true, false, true, true, false};

    for (auto x : make_iterable(source).map([](auto x) { return x; })) {
        x.flip();
    }

    ASSERT_EQ(source, expected_result);
}
