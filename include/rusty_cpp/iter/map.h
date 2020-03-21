#ifndef RUSTY_CPP_ITER_MAP_H
#define RUSTY_CPP_ITER_MAP_H

#include <iterator>

namespace rusty_cpp::iter::map {
template <class I, class F>
class Iterator {
    static_assert(std::is_object_v<I> && !std::is_const_v<I>);
    static_assert(std::is_lvalue_reference_v<F>);

    using MappedType = decltype(std::declval<const F &>()(*std::declval<I>()));

    I iter;
    F f;

    template <class C>
    using Category = C;

#if __cplusplus > 201703L
    template <>
    using Category<std::contiguous_iterator_tag> = std::random_access_iterator_tag;
#endif

public:
    using difference_type = typename std::iterator_traits<I>::difference_type;
    using value_type = std::remove_reference_t<MappedType>;
    using pointer = std::conditional_t<std::is_reference_v<MappedType>, value_type *, void>;
    using reference = std::conditional_t<std::is_reference_v<MappedType>, MappedType, void>;
    using iterator_category = Category<typename std::iterator_traits<I>::iterator_category>;

    Iterator(I iter, F f) : iter{iter}, f{f} {
    }

    bool operator!=(Iterator rhs) const {
        return this->iter != rhs.iter;
    }

    Iterator &operator++() {
        ++this->iter;

        return *this;
    }

    decltype(auto) operator*() const {
        return this->f(*this->iter);
    }

    auto operator-(Iterator rhs) const {
        return this->iter - rhs.iter;
    }

    auto operator-> () const {
        return pointer{&**this};
    }

    template <class K>
    decltype(auto) operator[](K &&key) const {
        return this->f(this->iter[std::forward<K>(key)]);
    }
};

template <class I, class F>
Iterator<I, F> make_iterator(I &&iter, F &&f) {
    return Iterator<I, F>{std::forward<I>(iter), std::forward<F>(f)};
}

template <class C, class F>
class Map {
    static_assert(std::is_lvalue_reference_v<C> || (std::is_object_v<C> && !std::is_const_v<C>));
    static_assert(std::is_lvalue_reference_v<F> || (std::is_object_v<F> && !std::is_const_v<F>));

    using InnerIterator = decltype(std::begin(std::declval<C &>()));
    using InnerConstIterator = decltype(std::begin(std::declval<const C &>()));

    C container;
    F f;

public:
    Map(C &&container, F &&f) : container{std::forward<C>(container)}, f{std::forward<F>(f)} {
    }

    auto begin() {
        return make_iterator(std::begin(this->container), this->f);
    }

    auto begin() const {
        return make_iterator(std::begin(this->container), this->f);
    }

    auto end() {
        return make_iterator(std::end(this->container), this->f);
    }

    auto end() const {
        return make_iterator(std::end(this->container), this->f);
    }

    auto size() const {
        return this->container.size();
    }
};

template <class C, class F>
Map<C, F> make_map(C &&container, F &&f) {
    return Map<C, F>{std::forward<C>(container), std::forward<F>(f)};
}
} // namespace rusty_cpp::iter::map

#endif // RUSTY_CPP_ITER_MAP_H
