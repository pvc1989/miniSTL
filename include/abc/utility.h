// Copyright 2019 Weicheng Pei

#ifndef PVC_UTILITY_H_
#define PVC_UTILITY_H_

#include "type_traits.h"

namespace pvc {

template <class T>
remove_reference_t<T>&& move(T&& t) {
  return static_cast<remove_reference_t<T>&&>(t);
}

template <class T>
T&& forward(remove_reference_t<T>&  t) { return static_cast<T&&>(t); }
template <class T>
T&& forward(remove_reference_t<T>&& t) { return static_cast<T&&>(t); }

}  // namespace pvc

#endif  // PVC_UTILITY_H_
