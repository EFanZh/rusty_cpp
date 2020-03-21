#ifndef RUSTY_CPP_ITER_ITERABLE_H
#define RUSTY_CPP_ITER_ITERABLE_H

#include <rusty_cpp/iter/map.h>

namespace rusty_cpp::iter::iterable {
template <class C>
class Iterable;

template <class C>
Iterable<C> make_iterable(C &&container) {
    return Iterable<C>{std::forward<C>(container)};
}

template <class C, class F>
Iterable<map::Map<C, F>> make_iterable_map(C &&container, F &&f) {
    return make_iterable(map::make_map(std::forward<C>(container), std::forward<F>(f)));
}

template <class C>
class Iterable {
    static_assert(std::is_lvalue_reference_v<C> || (std::is_object_v<C> && !std::is_const_v<C>));

    C container;

public:
    explicit Iterable(C &&container) : container{std::forward<C>(container)} {
    }

    // Standard methods.

    auto begin() {
        return std::begin(this->container);
    }

    auto begin() const {
        return std::begin(this->container);
    }

    auto end() {
        return std::end(this->container);
    }

    auto end() const {
        return std::end(this->container);
    }

    auto cbegin() const {
        return this->begin();
    }

    auto cend() const {
        return this->end();
    }

    auto size() const {
        return std::size(this->container);
    }

    template <class K>
    decltype(auto) operator[](K &&key) {
        return this->container[std::forward<K>(key)];
    }

    template <class K>
    decltype(auto) operator[](K &&key) const {
        return this->container[std::forward<K>(key)];
    }

    // Extension methods.

    template <class F>
    Iterable<map::Map<C &, F>> map(F &&f) & {
        return make_iterable_map(this->container, std::forward<F>(f));
    }

    template <class F>
    Iterable<map::Map<const C &, F>> map(F &&f) const & {
        return make_iterable_map(this->container, std::forward<F>(f));
    }

    template <class F>
    Iterable<map::Map<C, F>> map(F &&f) && {
        return make_iterable_map(std::forward<C>(this->container), std::forward<F>(f));
    }

    template <class F>
    Iterable<map::Map<C, F>> map(F &&f) const && = delete;
};
} // namespace rusty_cpp::iter::iterable

#endif // RUSTY_CPP_ITER_ITERABLE_H
