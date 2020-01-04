// Copyright 2019 Weicheng Pei
#ifndef ABC_ITERATOR_H_
#define ABC_ITERATOR_H_

#include <cstddef>

namespace abc {

template <class Category,
          class T,
          class Distance = std::ptrdiff_t,
          class Pointer = T*,
          class Reference = T&
> struct iterator {
  using iterator_category = Category;
  using value_type = T;
  using difference_type = Distance;
  using pointer = Pointer;
  using reference = Reference;
};

}  // namespace abc

#endif  // ABC_ITERATOR_H_
