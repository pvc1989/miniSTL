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

#ifdef PVC_USE_SMART_POINTER_
 public:  // iterator and related methods
  struct iterator : public pvc::iterator<
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
      ptr_node = ptr_node->ptr_next.get();
      return *this;
    }
    iterator operator++(int) noexcept {
      auto iter = iterator(ptr_node);
      ptr_node = ptr_node->ptr_next.get();
      return iter;
    }
  };

  iterator begin() noexcept { return ptr_head_.get(); };
  iterator end() noexcept { return nullptr; }

  struct const_iterator : public iterator {
    friend forward_list;
   public:
    using reference = typename forward_list::const_reference;
    using pointer = typename forward_list::const_pointer;

    const_iterator(Node* ptr_node) noexcept : iterator(ptr_node) { }

    reference operator*() const noexcept { return this->iterator::operator*(); }
    pointer operator->() const noexcept { return this->iterator::operator->(); }
  };

  const_iterator cbegin() const noexcept { return ptr_head_.get(); };
  const_iterator cend() const noexcept { return nullptr; }

  const_iterator begin() const noexcept { return cbegin(); };
  const_iterator end() const noexcept { return cend(); }

 public:  // modifying methods
  template <class... Args>
  void emplace_front(Args&&... args) {
    auto ptr_new = std::make_unique<Node>(std::forward<Args>(args)...);
    ptr_new->ptr_next.reset(ptr_head_.release());
    ptr_new.swap(ptr_head_);
  }

  void pop_front() noexcept {
    auto ptr_next = ptr_head_->ptr_next.release();
    ptr_head_.reset(ptr_next);
  }

  template <class... Args> 
  iterator emplace_after(iterator pos, Args&&... args) {
    auto& ptr_next = pos.ptr_node->ptr_next;
    auto ptr_new = std::make_unique<Node>(std::forward<Args>(args)...);
    ptr_new->ptr_next.reset(ptr_next.release());
    ptr_next.reset(ptr_new.release());
    return iterator(ptr_next.get());
  }
};

#else

 public:  // iterator and related methods
  struct iterator : public pvc::iterator<
      std::forward_iterator_tag, forward_list::value_type> {
    friend forward_list;
   private:
    Node* ptr_node{ nullptr };
   public:
    iterator(Node* ptr_node) noexcept : ptr_node(ptr_node) { }

    reference operator*() const noexcept {
      return ptr_node->value;
    }
    pointer operator->() const noexcept {
      return &this->operator*();
    }

    bool operator==(iterator const& rhs) const noexcept {
      return ptr_node == rhs.ptr_node;
    }
    bool operator!=(iterator const& rhs) const noexcept {
      return !(*this == rhs);
    }

    iterator& operator++() noexcept {
      ptr_node = ptr_node->ptr_next;
      return *this;
    }
    iterator operator++(int) noexcept {
      auto iter_old = iterator(ptr_node);
      ptr_node = ptr_node->ptr_next;
      return iter_old;
    }
  };

  iterator begin() noexcept { return ptr_head_; }
  iterator end() noexcept { return nullptr; }

  struct const_iterator : public iterator {
    friend forward_list;
   public:
    using reference = typename forward_list::const_reference;
    using pointer = typename forward_list::const_pointer;

    const_iterator(Node* ptr_node) noexcept : iterator(ptr_node) { }

    reference operator*() const noexcept { return this->iterator::operator*(); }
    pointer operator->() const noexcept { return this->iterator::operator->(); }
  };

  const_iterator cbegin() const noexcept { return ptr_head_; };
  const_iterator cend() const noexcept { return nullptr; }

  const_iterator begin() const noexcept { return cbegin(); };
  const_iterator end() const noexcept { return cend(); }

 public:  // modifying methods
  template <class... Args>
  void emplace_front(Args&&... args) {
    ptr_head_ = new Node(ptr_head_, std::forward<Args>(args)...);
  }

  void pop_front() noexcept {
    auto ptr_old = ptr_head_;
    ptr_head_ = ptr_head_->ptr_next;
    delete ptr_old;
  }

  template <class... Args> 
  iterator emplace_after(iterator pos, Args&&... args) {
    auto& pos_next = pos.ptr_node->ptr_next;
    auto ptr_new = new Node(pos_next, std::forward<Args>(args)...);
    pos_next = ptr_new;
    return ++pos;
  }
};

#endif

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
