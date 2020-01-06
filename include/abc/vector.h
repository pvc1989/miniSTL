// Copyright 2019 Minghao Yang and Weicheng Pei
#ifndef ABC_VECTOR_H_
#define ABC_VECTOR_H_

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>

#include "abc/iterator.h"
#include "abc/utility.h"

namespace abc {

template <class T, class Allocator = std::allocator<T>>
class vector {
 public:
  using size_type = std::size_t;
  using reference = T&;
  using const_reference = const T&;
  using pointer = T*;
  using const_pointer = const T*;

 public:
  // construction
  vector() = default;
  explicit vector(size_type count, const T& value = T())
      : capacity_(count), size_(count), array_(allocator_.allocate(count)) {
    std::uninitialized_fill_n(array_, size_, value);
  }
  vector(std::initializer_list<T> init) {
    capacity_ = init.end() - init.begin();
    size_ = capacity_;
    array_ = allocator_.allocate(size_);
    std::uninitialized_copy(init.begin(), init.end(), array_);
  }
  vector& operator=(std::initializer_list<T> init) {
    clear();
    size_ = init.end() - init.begin();
    if (size_ > capacity_) {
      allocator_.deallocate(array_, capacity_);
      capacity_ = size_;
      array_ = allocator_.allocate(capacity_);
    }
    std::uninitialized_copy(init.begin(), init.end(), array_);
    return *this;
  }
  // destruction
  ~vector() noexcept {
    clear();
    allocator_.deallocate(array_, capacity_);
  }
  // copy operations:
  vector(const vector& that) { *this = that; }
  vector& operator=(const vector& that) {
    if (this != &that) {
      clear();
      if (capacity_ != that.capacity()) {
        allocator_.deallocate(array_, capacity_);
        capacity_ = that.capacity();
      }
      size_ = that.size();
      std::uninitialized_copy(that.begin(), that.end(), array_);
    }
    return *this;
  }
  // move operations:
  vector(vector&& that) { *this = std::move(that); }
  vector& operator=(vector&& that) {
    if (this != &that) {
      capacity_ = that.capacity();
      size_ = that.size();
      array_ = allocator_.allocate(capacity_);
      std::uninitialized_move(that.begin(), that.end(), array_);
    }
    return *this;
  }

 private:  // Data members:
  // Don't change the order of these members!
  size_type capacity_{0};
  size_type size_{0};
  T* array_{allocator_.allocate(0)};
  static Allocator allocator_;

 public:
  // iterator and related methods
  using iterator = pointer;

  iterator begin() noexcept { return array_; };
  iterator end() noexcept { return array_+size_; }

  using const_iterator = const_pointer;

  const_iterator cbegin() const noexcept { return array_; };
  const_iterator cend() const noexcept { return array_+size_; }

  const_iterator begin() const noexcept { return cbegin(); };
  const_iterator end() const noexcept { return cend(); }

  // non-modifying methods
  bool empty() const noexcept {
    return size_ == 0;
    // return begin() == end();
  }
  reference front() { return array_[0]; }
  reference back() { return array_[size_ - 1]; }
  size_type size() const { return size_; }
  size_type capacity() const { return capacity_; }
  T& operator[] (size_type pos) {
    return array_[pos];
  }
  const T& operator[] (size_type pos) const {
    return array_[pos];
  }
  T& at(size_type pos) {
    if (pos >= size_) {
      std::out_of_range("Out of range!");
    }
    return array_[pos];
  }
  const T& at(size_type pos) const {
    if (pos >= size_) {
      std::out_of_range("Out of range!");
    }
    return array_[pos];
  }

  // modifying methods
  void resize(size_type count, const T& value = T()) {
    if (count > capacity_) {
      auto old_capacity = capacity_;
      while (capacity_ < count) { capacity_ *= 2; }
      auto new_array_ = allocator_.allocate(capacity_);
      std::uninitialized_move(array_, array_+size_, new_array_);
      std::uninitialized_fill_n(new_array_+size_, count-size_, value);
      std::swap(new_array_, array_);
      allocator_.deallocate(new_array_, old_capacity);
    } else if (count > size_) {
      std::uninitialized_fill_n(array_+size_, count-size_, value);
    }
    size_ = count;
  }
  template <class... Args>
  void emplace_back(Args&&... args) {
    if (size() == capacity()) {
      enlarge();
    }
    assert(size() < capacity());
    allocator_.construct(&array_[size_++], std::forward<Args>(args)...);
  }
  void push_back(const T& value) {
    if (size() == capacity()) {
      enlarge();
    }
    assert(size() < capacity());
    allocator_.construct(&array_[size_++], value);
  }
  void push_back(T&& value) {
    if (size() == capacity()) {
      enlarge();
    }
    assert(size() < capacity());
    allocator_.construct(&array_[size_++], std::move(value));
  }
  void pop_back() noexcept {
    size_--;
    if (size_ > 0 && size_ <= capacity_/4) {
      shrink();
    }
  }
  void clear() noexcept {
    for (int i = 0; i < size_; i++) {
      allocator_.destroy(array_ + i);
    }
    size_ = 0;
  }
  void swap(vector& other) {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(array_, other.array_);
  }

 private:
  void enlarge() {
    auto old_capacity = capacity_;
    if (size_ == 0) {
      capacity_ = 1;
    } else {
      capacity_ = size_ * 2;
    }
    auto new_array_ = allocator_.allocate(capacity_);
    std::uninitialized_move(array_, array_+size_, new_array_);
    std::swap(array_, new_array_);
    allocator_.deallocate(new_array_, old_capacity);
  }
  void shrink() {
    auto old_capacity = capacity_;
    capacity_ /= 2;
    auto new_array_ = allocator_.allocate(capacity_);
    std::uninitialized_move(array_, array_+size_, new_array_);
    std::swap(array_, new_array_);
    allocator_.deallocate(new_array_, old_capacity);
  }
};
template <class T, class Allocator>
Allocator vector<T, Allocator>::allocator_;

}  // namespace abc

template <class T, class Allocator>
bool operator==(const abc::vector<T, Allocator>& lhs,
                const abc::vector<T, Allocator>& rhs) noexcept {
  auto iter = lhs.begin();
  const auto iend = lhs.end();
  for (const auto& x : rhs) {
    if (iter == iend || *iter != x) {
      return false;
    } else {
      ++iter;
    }
  }
  return iter == iend;
}
template <class T, class Allocator>
bool operator!=(const abc::vector<T, Allocator>& lhs,
                const abc::vector<T, Allocator>& rhs) noexcept {
  return !(lhs == rhs);
}

#endif  // ABC_VECTOR_H_
