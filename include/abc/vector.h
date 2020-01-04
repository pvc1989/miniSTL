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

template <class T>
class vector {
 public:
  using size_type = std::size_t;
  using reference = T&;
  using const_reference = const T&;
  using pointer = T*;
  using const_pointer = const T*;

 public:
  // construction
  vector() : capacity_(0), size_(0), array_(new T[0]) {}
  explicit vector(size_type count) : capacity_(count), size_(count),
                                     array_(new T[count]) {}
  explicit vector(size_type count, const T& value) : capacity_(count),
                                                     size_(count),
                                                     array_(new T[count]) {
    for (int i = 0; i < size_; i++) {
      array_[i] = value;
    }
  }
  vector(std::initializer_list<T> init) {
    capacity_ = init.end() - init.begin();
    size_ = capacity_;
    array_ = std::make_unique<T[]>(size_);
    auto iter = init.begin();
    for (int i = 0; i < size_; i++) {
      array_[i] = *iter;
      iter++;
    }
  }
  vector& operator=(std::initializer_list<T> init) {
    size_ = init.end() - init.begin();
    capacity_ = std::max(size_, capacity_);
    array_ = std::make_unique<T[]>(size_);
    auto iter = init.begin();
    for (int i = 0; i < size_; i++) {
      array_[i] = *iter;
      iter++;
    }
    return *this;
  }
  // destruction
  ~vector() noexcept { clear(); }
  // copy operations:
  vector(const vector& that) { *this = that; }
  vector& operator=(const vector& that) {
    if (this != &that) {
      clear();
      capacity_ = that.capacity();
      size_ = that.size();
      array_ = std::make_unique<T[]>(size_);
      for (int i = 0; i < size_; i++) {
        array_[i] = that[i];
      }
    }
    return *this;
  }
  // move operations:
  vector(vector&& that) { *this = std::move(that); }
  vector& operator=(vector&& that) {
    if (this != &that) {
      clear();
      capacity_ = that.capacity();
      size_ = that.size();
      array_.swap(that.array_);
    }
    return *this;
  }

 private:
  size_type capacity_;
  size_type size_;
  std::unique_ptr<T[]> array_;

 public:
  // iterator and related methods
  using iterator = pointer;

  iterator begin() noexcept { return array_.get(); };
  iterator end() noexcept { return array_.get()+size_; }

  using const_iterator = const_pointer;

  const_iterator cbegin() const noexcept { return array_.get(); };
  const_iterator cend() const noexcept { return array_.get()+size_; }

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
  void resize(size_type count, const T& value) {
    if (count > capacity_) {
      while (capacity_ < count) { capacity_ *= 2; }
      auto new_array_ = std::make_unique<T[]>(capacity_);
      for (int i = size_; i < count; i++) {
        new_array_[i] = value;
      }
      for (int i = 0; i < size_; i++) {
        std::swap(new_array_[i], array_[i]);
      }
      array_.swap(new_array_);
    } else if (count > size_) {
      for (int i = size_; i < count; i++) {
        array_[i] = value;
      }
    }
    size_ = count;
  }
  void resize(size_type count) {
    resize(count, T());
  }
  template <class... Args>
  void emplace_back(Args&&... args) {
    if (size() == capacity()) {
      enlarge();
    }
    assert(size() < capacity());
    array_[size_++] = T(std::forward<Args>(args)...);
  }
  void push_back(const T& value) {
    if (size() == capacity()) {
      enlarge();
    }
    assert(size() < capacity());
    array_[size_++] = value;
  }
  void push_back(T&& value) {
    if (size() == capacity()) {
      enlarge();
    }
    assert(size() < capacity());
    array_[size_++] = std::move(value);
  }
  void pop_back() noexcept {
    size_--;
    if (size_ > 0 && size_ <= capacity_/4) {
      shrink();
    }
  }
  void clear() noexcept {
    size_ = 0;
    array_.reset(new T[0]);
  }
  void swap(vector& other) {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    array_.swap(other.array_);
  }

 private:
  void enlarge() {
    if (size_ == 0) {
      capacity_ = 1;
    } else {
      capacity_ = size_ * 2;
    }
    auto new_array_ = std::make_unique<T[]>(capacity_);
    for (int i = 0; i < size_; i++) {
      std::swap(new_array_[i], array_[i]);
    }
    array_.swap(new_array_);
  }
  void shrink() {
    capacity_ /= 2;
    auto new_array_ = std::make_unique<T[]>(capacity_);
    for (int i = 0; i < size_; i++) {
      std::swap(new_array_[i], array_[i]);
    }
    array_.swap(new_array_);
  }
};

}  // namespace abc

template <class T>
bool operator==(const abc::vector<T>& lhs,
                const abc::vector<T>& rhs) noexcept {
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
template <class T>
bool operator!=(const abc::vector<T>& lhs,
                const abc::vector<T>& rhs) noexcept {
  return !(lhs == rhs);
}

#endif  // ABC_VECTOR_H_
