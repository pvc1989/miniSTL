// Copyright 2019 Weicheng Pei
#ifndef ABC_UTILITY_H_
#define ABC_UTILITY_H_

#include "type_traits.h"

namespace abc {

template <class T>
remove_reference_t<T>&& move(T&& t) {
  return static_cast<remove_reference_t<T>&&>(t);
}

template <class T>
T&& forward(remove_reference_t<T>&  t) { return static_cast<T&&>(t); }
template <class T>
T&& forward(remove_reference_t<T>&& t) { return static_cast<T&&>(t); }

}  // namespace abc

#endif  // ABC_UTILITY_H_
