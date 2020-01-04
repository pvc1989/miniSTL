// Copyright 2019 Weicheng Pei

#ifndef PVC_FORWARD_LIST_H_
#define PVC_FORWARD_LIST_H_

#include <cstddef>
#include <memory>

#include "iterator.h"
#include "utility.h"

namespace pvc {

template <class T>
class forward_list {
 public:
  using value_type = T;
  using size_type = std::size_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;

 public:
  forward_list() = default;
  ~forward_list() noexcept { clear(); }
  // copy operations:
  forward_list(const forward_list& that) { *this = that; }
  forward_list& operator=(const forward_list& that) {
    if (this != &that) {
      clear();
      auto iter_of_that = that.begin();
      auto iend_of_that = that.end();
      // add the 1st item, if any:
      if (iter_of_that != iend_of_that) {
        emplace_front(*iter_of_that);
        ++iter_of_that;
      }
      // add the other items, if any:
      auto iter_of_this = begin();
      while (iter_of_that != iend_of_that) {
        iter_of_this = emplace_after(iter_of_this, *iter_of_that);
        ++iter_of_that;
      }
    }
    return *this;
  }
  // move operations:
  forward_list(forward_list&& that) { *this = std::move(that); }
  forward_list& operator=(forward_list&& that) {
    if (this != &that) {
      clear();
      std::swap(this->ptr_head_, that.ptr_head_);
    }
    return *this;
  }
  // accessors:
  bool empty() const noexcept { return !ptr_head_; }
  reference front() { return ptr_head_->value; }
  const_reference front() const { return ptr_head_->value; }
  // mutators:
  void clear() noexcept {
    while(!empty()) {
      pop_front();
    }
  }

 private:
  struct Node {
   public:  // type member:
#ifdef PVC_USE_SMART_POINTER_
    using Pointer = std::unique_ptr<Node>;
#else
    using Pointer = Node*;
#endif
   public:  // data members:
    value_type value;
    Pointer ptr_next;
   public:  // constuctors:
    template <class... Args>
    explicit Node(Args&&... args) : value(std::forward<Args>(args)...) { }
    template <class... Args>
    explicit Node(Node* ptr_node, Args&&... args)
      : ptr_next(ptr_node), value(std::forward<Args>(args)...) { }
  };

 private:
  using NodePtr = typename Node::Pointer;
  NodePtr ptr_head_{ nullptr };  // the only data member of forward_list<T>

 public:  // operations at the beginning:
  template <class... Args>
  void emplace_front(Args&&... args) {
#ifdef PVC_USE_SMART_POINTER_
    auto ptr_new = std::make_unique<Node>(
      ptr_head_.release()/* raw pointer of the old head */,
      std::forward<Args>(args).../* arguments for value */);
    ptr_new.swap(ptr_head_);
#else
    ptr_head_ = new Node(ptr_head_, std::forward<Args>(args)...);
#endif
  }
  void pop_front() noexcept {
#ifdef PVC_USE_SMART_POINTER_
    auto ptr_next = ptr_head_->ptr_next.release();
    ptr_head_.reset(ptr_next);
#else
    auto ptr_old = ptr_head_;
    ptr_head_ = ptr_head_->ptr_next;
    delete ptr_old;
#endif
  }

 public:  // iterators and related methods
  class iterator : public pvc::iterator<
      std::forward_iterator_tag, forward_list::value_type> {
    friend forward_list;
   protected:
    Node* ptr_node{ nullptr };
   public:
    iterator(Node* ptr_node) noexcept : ptr_node(ptr_node) { }
    reference operator*() const noexcept { return ptr_node->value; }
    pointer operator->() const noexcept { return &this->operator*(); }
    bool operator==(iterator const& rhs) const noexcept {
      return ptr_node == rhs.ptr_node;
    }
    bool operator!=(iterator const& rhs) const noexcept {
      return !(*this == rhs);
    }
    iterator& operator++() noexcept {
      ptr_node = &(*(ptr_node->ptr_next));
      return *this;
    }
    iterator operator++(int) noexcept {
      auto iter = iterator(ptr_node);
      ptr_node = &(*(ptr_node->ptr_next));
      return iter;
    }
  };  // iterator
  class const_iterator : public iterator {
    friend forward_list;
   public:
    using reference = typename forward_list::const_reference;
    using pointer = typename forward_list::const_pointer;
    const_iterator(Node* ptr_node) noexcept : iterator(ptr_node) { }
    reference operator*() const noexcept { return this->iterator::operator*(); }
    pointer operator->() const noexcept { return this->iterator::operator->(); }
  };  // const_iterator
  // range related methods:
  iterator begin() noexcept { return &(*ptr_head_); }
  const_iterator begin() const noexcept { return cbegin(); };
  const_iterator cbegin() const noexcept { return &(*ptr_head_); };
  iterator end() noexcept { return nullptr; }
  const_iterator end() const noexcept { return cend(); }
  const_iterator cend() const noexcept { return nullptr; }
  // construct a new element after an element given by an iterator:
  template <class... Args> 
  iterator emplace_after(iterator iter, Args&&... args) {
#ifdef PVC_USE_SMART_POINTER_
    auto& ptr_next = iter.ptr_node->ptr_next;
    auto ptr_new = std::make_unique<Node>(
      ptr_next.release()/* raw pointer of the old head */,
      std::forward<Args>(args).../* arguments for value */);
    ptr_next.reset(ptr_new.release());
#else
    auto& ptr_next = iter.ptr_node->ptr_next;
    auto ptr_new = new Node(ptr_next, std::forward<Args>(args)...);
    ptr_next = ptr_new;
#endif
    return ++iter;
  }
};  // forward_list

template <class T>
bool operator==(const forward_list<T>& lhs,
                const forward_list<T>& rhs) noexcept {
  auto iter = lhs.begin();
  const auto iend = lhs.end();
  for (const auto& x : rhs) {
    if (iter == iend || *iter != x) { return false; }
    else { ++iter; }
  }
  if (iter == iend) { return true; }
  else { return false; }  
}
template <class T>
bool operator!=(const forward_list<T>& lhs,
                const forward_list<T>& rhs) noexcept {
  return !(lhs == rhs);                 
}

}  // namespace pvc

#endif  // PVC_FORWARD_LIST_H_
