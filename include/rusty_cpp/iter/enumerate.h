#ifndef RUSTY_CPP_ITER_ENUMERATE_H
#define RUSTY_CPP_ITER_ENUMERATE_H

#include <iterator>
#include <rusty_cpp/iter/utilities.h>
#include <tuple>

namespace rusty_cpp::iter::enumerate {
template <class I, class N>
class Iterator {
    static_assert(utilities::IsGoodObjectMemberV<I>);
    static_assert(utilities::IsGoodObjectMemberV<N>);

    I iter;
    N count;

    using BaseCategory = typename std::iterator_traits<I>::iterator_category;

public:
    using difference_type = typename std::iterator_traits<I>::difference_type;
    using value_type = std::tuple<N, typename std::iterator_traits<I>::reference>;
    using pointer = void;
    using reference = value_type;

    using iterator_category = std::conditional_t<std::is_base_of_v<std::random_access_iterator_tag, BaseCategory>,
                                                 std::random_access_iterator_tag,
                                                 BaseCategory>;

    Iterator(I iter, N count) : iter{iter}, count{count} {
    }

    bool operator==(Iterator rhs) const {
        return this->iter == rhs.iter;
    }

    bool operator!=(Iterator rhs) const {
        return this->iter != rhs.iter;
    }

    Iterator &operator++() {
        ++this->iter;
        ++this->count;

        return *this;
    }

    reference operator*() const {
        return reference{this->count, *this->iter};
    }

    auto operator-(Iterator rhs) const {
        return this->iter - rhs.iter;
    }

    auto operator[](difference_type key) const {
        return std::make_tuple(this->count + static_cast<N>(key), this->iter[std::forward<K>(key)]);
    }
};

template <class I, class N>
Iterator<I, N> make_iterator(I iter, N count) {
    return Iterator<I, N>{iter, count};
}

template <class C, class N>
class Enumerate {
    static_assert(std::is_lvalue_reference_v<C> || (std::is_object_v<C> && !std::is_const_v<C>));
    static_assert(std::is_object_v<N> && !std::is_const_v<N>);

    C container;
    N count;

public:
    Enumerate(C &&container, N &&count) : container{std::forward<C>(container)}, count{std::forward<N>(count)} {
    }

    auto begin() {
        return make_iterator(std::begin(this->container), std::forward<N>(this->count));
    }

    auto begin() const {
        return make_iterator(std::begin(this->container), std::forward<const N>(this->count));
    }

    auto end() {
        return make_iterator(std::end(this->container), std::forward<N>(this->count));
    }

    auto end() const {
        return make_iterator(std::end(this->container), std::forward<const N>(this->count));
    }

    auto size() const {
        return this->container.size();
    }
};

template <class C, class N>
Enumerate<C, N> make_enumerate(C &&container, N &&count) {
    return Enumerate<C, N>{std::forward<C>(container), std::forward<N>(count)};
};
} // namespace rusty_cpp::iter::enumerate

#endif // RUSTY_CPP_ITER_ENUMERATE_H
