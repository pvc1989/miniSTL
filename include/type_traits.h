// Copyright 2019 Weicheng Pei

#ifndef PVC_TYPE_TRAITS_H_
#define PVC_TYPE_TRAITS_H_

namespace pvc {

template <class T> struct remove_reference      { using type = T; };
template <class T> struct remove_reference<T&>  { using type = T; };
template <class T> struct remove_reference<T&&> { using type = T; };

template <class T>
using remove_reference_t = typename remove_reference<T>::type;

}  // namespace pvc

#endif  // PVC_TYPE_TRAITS_H_
