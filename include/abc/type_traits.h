// Copyright 2019 Weicheng Pei
#ifndef ABC_TYPE_TRAITS_H_
#define ABC_TYPE_TRAITS_H_

namespace abc {

template <class T> struct remove_reference      { using type = T; };
template <class T> struct remove_reference<T&>  { using type = T; };
template <class T> struct remove_reference<T&&> { using type = T; };

template <class T>
using remove_reference_t = typename remove_reference<T>::type;

}  // namespace abc

#endif  // ABC_TYPE_TRAITS_H_
