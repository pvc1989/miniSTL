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
  using reference = value_type&;

 public:
  ~forward_list() noexcept { clear(); }

 private:
  struct Node {
    value_type value;
    std::unique_ptr<Node> uptr_next;

    template <class... Args>
    explicit Node(Args... args) : value(std::forward<Args>(args)...) { }
  };

 public:  // iterator and related methods
  struct iterator : public std::iterator<
      std::forward_iterator_tag, value_type, std::ptrdiff_t> {
    friend forward_list;
   public:
    explicit iterator(Node* ptr_node) noexcept : ptr_node(ptr_node) { }

    typename iterator::reference operator*() const noexcept {
      return ptr_node->value;
    }
    typename iterator::pointer operator->() const noexcept {
      return &this->operator*();
    }

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

   private:
    Node* ptr_node{ nullptr };
  };

  iterator begin() noexcept {
    return iterator(uptr_head_.get());
  };
  iterator end() noexcept {
    return iterator(nullptr);
  }

 public:  // non-modifying methods
  bool empty() const noexcept { return !uptr_head_.get(); }

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

 private:
  std::unique_ptr<Node> uptr_head_{ nullptr };
};

// using raw pointers
namespace raw {

template <class T>
class forward_list {
 public:
  using value_type = T;
  using size_type = std::size_t;
  using reference = value_type&;

 public:
  ~forward_list() noexcept { clear(); }

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
    explicit iterator(Node* ptr_node) noexcept : ptr_node(ptr_node) { }

    typename iterator::reference operator*() const noexcept {
      return ptr_node->value;
    }
    typename iterator::pointer operator->() const noexcept {
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

  iterator begin() noexcept {
    return iterator(ptr_head_);
  };
  iterator end() noexcept {
    return iterator(nullptr);
  }

 public:  // non-modifying methods
  bool empty() const noexcept { return !ptr_head_; }

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
