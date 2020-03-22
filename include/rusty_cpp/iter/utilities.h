#ifndef RUSTY_CPP_ITER_UTILITIES_H
#define RUSTY_CPP_ITER_UTILITIES_H

#include <type_traits>

namespace rusty_cpp::iter::utilities {
// Stuff from newer C++ standards.

template <class T>
struct IsUnboundedArray : std::false_type {
};

template <class T>
struct IsUnboundedArray<T[]> : std::true_type {
};

template <class T>
inline constexpr bool IsUnboundedArrayV = IsUnboundedArray<T>::value;

template <class T>
struct RemoveCVRef {
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

template <class T>
using RemoveCVRefT = typename RemoveCVRef<T>::type;

// My stuff.

template <class T>
inline constexpr bool IsGoodObjectMemberV = std::is_object_v<T> && !IsUnboundedArrayV<T> && !std::is_const_v<T>;

template <class T>
inline constexpr bool IsGoodMemberV = std::is_lvalue_reference_v<T> || IsGoodObjectMemberV<T>;
} // namespace rusty_cpp::iter::utilities

#endif // RUSTY_CPP_ITER_UTILITIES_H
