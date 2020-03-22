#ifndef RUSTY_CPP_ITER_UTILITIES_H
#define RUSTY_CPP_ITER_UTILITIES_H

#include <type_traits>

namespace rusty_cpp::iter::utilities {
template <class T>
inline constexpr bool IsGoodObjectMemberV = std::is_object_v<T> && !std::is_unbounded_array_v<T> && !std::is_const_v<T>;

template <class T>
inline constexpr bool IsGoodMemberV = std::is_lvalue_reference_v<T> || IsGoodObjectMemberV<T>;
} // namespace rusty_cpp::iter::utilities

#endif // RUSTY_CPP_ITER_UTILITIES_H
