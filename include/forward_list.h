// Copyright 2019 Weicheng Pei

#ifndef PVC_FORWARD_LIST_H_
#define PVC_FORWARD_LIST_H_

#include <iterator>
#include <memory>
#include <utility>

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
      auto iter_on_that = that.begin();
      auto iend_of_that = that.end();
      // add the 1st item, if any:
      if (iter_on_that != iend_of_that) {
        emplace_front(*iter_on_that);
        ++iter_on_that;
      }
      // add the other items, if any:
      auto iter_on_this = begin();
      while (iter_on_that != iend_of_that) {
        iter_on_this = emplace_after(iter_on_this, *iter_on_that);
        ++iter_on_that;
      }
    }
    return *this;
  }
  // move operations:
  forward_list(forward_list&& that) { *this = std::move(that); }
  forward_list& operator=(forward_list&& that) {
    if (this != &that) {
      clear();
      uptr_head_.swap(that.uptr_head_);
    }
    return *this;
  }

 private:
  struct Node {
    value_type value;
    std::unique_ptr<Node> uptr_next;

    template <class... Args>
    explicit Node(Args... args) : value(std::forward<Args>(args)...) { }
  };

 private:
  std::unique_ptr<Node> uptr_head_{ nullptr };

 public:  // iterator and related methods
  struct iterator : public std::iterator<
      std::forward_iterator_tag, value_type, std::ptrdiff_t> {
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
      ptr_node = ptr_node->uptr_next.get();
      return *this;
    }
    iterator operator++(int) noexcept {
      auto iter = iterator(ptr_node);
      ptr_node = ptr_node->uptr_next.get();
      return iter;
    }
  };

  iterator begin() noexcept { return uptr_head_.get(); };
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

  const_iterator cbegin() const noexcept { return uptr_head_.get(); };
  const_iterator cend() const noexcept { return nullptr; }

  const_iterator begin() const noexcept { return cbegin(); };
  const_iterator end() const noexcept { return cend(); }

 public:  // non-modifying methods
  bool empty() const noexcept { return !uptr_head_.get(); }

  template <class List>
  bool operator==(const List& that) const noexcept {
    auto iter = begin();
    const auto iend = end();
    for (const auto& x : that) {
      if (iter == iend || *iter != x) { return false; }
      else { ++iter; }
    }
    if (iter == iend) { return true; }
    else { return false; }
  }
  template <class List>
  bool operator!=(const List& that) const noexcept { return !(*this == that); }

 public:  // modifying methods
  template <class... Args>
  void emplace_front(Args&&... args) {
    auto uptr_new = std::make_unique<Node>(std::forward<Args>(args)...);
    uptr_new->uptr_next.reset(uptr_head_.release());
    uptr_new.swap(uptr_head_);
  }

  reference front() { return uptr_head_->value; }

  void pop_front() noexcept {
    auto ptr_next = uptr_head_->uptr_next.release();
    uptr_head_.reset(ptr_next);
  }

  void clear() noexcept {
    while(!empty()) {
      pop_front();
    }
  }

  template <class... Args> 
  iterator emplace_after(iterator pos, Args&&... args) {
    auto& uptr_next = pos.ptr_node->uptr_next;
    auto uptr_new = std::make_unique<Node>(std::forward<Args>(args)...);
    uptr_new->uptr_next.reset(uptr_next.release());
    uptr_next.reset(uptr_new.release());
    return iterator(uptr_next.get());
  }
};

// using raw pointers
namespace raw {

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
      auto iter_on_that = that.begin();
      auto iend_of_that = that.end();
      // add the 1st item, if any:
      if (iter_on_that != iend_of_that) {
        emplace_front(*iter_on_that);
        ++iter_on_that;
      }
      // add the other items, if any:
      auto iter_on_this = begin();
      while (iter_on_that != iend_of_that) {
        iter_on_this = emplace_after(iter_on_this, *iter_on_that);
        ++iter_on_that;
      }
    }
    return *this;
  }
  // move operations:
  forward_list(forward_list&& that) { *this = std::move(that); }
  forward_list& operator=(forward_list&& that) {
    if (this != &that) {
      clear();
      ptr_head_ = that.ptr_head_;
      that.ptr_head_ = nullptr;
    }
    return *this;
  }

 private:
  struct Node {
    Node* ptr_next{ nullptr };
    value_type value;

    template <class... Args>
    explicit Node(Node* ptr_node, Args... args)
      : ptr_next(ptr_node), value(std::forward<Args>(args)...) { }
  };

 private:
  Node* ptr_head_{ nullptr };

 public:  // iterator and related methods
  struct iterator : public std::iterator<
      std::forward_iterator_tag, value_type, std::ptrdiff_t> {
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

 public:  // non-modifying methods
  bool empty() const noexcept { return !ptr_head_; }

  template <class List>
  bool operator==(const List& that) const noexcept {
    auto iter = begin();
    const auto iend = end();
    for (const auto& x : that) {
      if (iter == iend || *iter != x) { return false; }
      else { ++iter; }
    }
    if (iter == iend) { return true; }
    else { return false; }
  }
  template <class List>
  bool operator!=(const List& that) const noexcept { return !(*this == that); }

 public:  // modifying methods
  template <class... Args>
  void emplace_front(Args&&... args) {
    ptr_head_ = new Node(ptr_head_, std::forward<Args>(args)...);
  }

  reference front() { return ptr_head_->value; }

  void pop_front() noexcept {
    auto ptr_old = ptr_head_;
    ptr_head_ = ptr_head_->ptr_next;
    delete ptr_old;
  }

  void clear() noexcept {
    while(!empty()) {
      pop_front();
    }
  }

  template <class... Args> 
  iterator emplace_after(iterator pos, Args&&... args) {
    auto& pos_next = pos.ptr_node->ptr_next;
    auto ptr_new = new Node(pos_next, std::forward<Args>(args)...);
    pos_next = ptr_new;
    return ++pos;
  }
};

}  // namespace raw
}  // namespace pvc
#endif  // PVC_FORWARD_LIST_H_
