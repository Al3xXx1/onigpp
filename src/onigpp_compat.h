#pragma once

// C++11 向け互換ヘッダ
// std::make_unique が存在しない（C++ < 14）場合にのみ定義します。

#include <memory>
#include <type_traits>
#include <utility>
#include <cstddef>

#if !defined(__cplusplus) || __cplusplus < 201402L

namespace std {

template<class T, class... Args>
typename std::enable_if<!std::is_array<T>::value, std::unique_ptr<T>>::type
make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// for arrays of unknown bound
template<class T>
typename std::enable_if<std::is_array<T>::value && std::extent<T>::value == 0, std::unique_ptr<T>>::type
make_unique(std::size_t n) {
    using U = typename std::remove_extent<T>::type;
    return std::unique_ptr<T>(new U[n]());
}

// for arrays of known bound - deleted
template<class T, class... Args>
typename std::enable_if<std::is_array<T>::value && std::extent<T>::value != 0>::type
make_unique(Args&&...) = delete;

} // namespace std

#endif // C++14 check
