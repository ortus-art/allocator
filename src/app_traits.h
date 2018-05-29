#pragma once

#include <type_traits>
#include <map>

#include "linked_list.h"

namespace app {

template <typename T>
constexpr auto is_integral_v = std::is_integral<std::remove_cv_t<T>>::value;

template<typename T>
using enable_if_integral_t =
    typename std::enable_if_t<is_integral_v<T>, int>;


} //namespace app
