// Copyright abc
#ifndef ABC_LIST_H_
#define ABC_LIST_H_

#include <cassert>
#include <cstddef>
#include <memory>

#include "abc/iterator.h"
#include "abc/utility.h"

namespace abc {

template <class T>
class list {
 public:
  using value_type = T;
  using size_type = std::size_t;

 public:
  list() = default;
  ~list() noexcept { clear(); }
  list(const list& that) { *this = that; }
  list& operator=(const list& that) {
    if (&that != this) {
      clear();
      auto iter_begin = that.begin();
      auto iter_end = that.end();
      for (auto iter = iter_begin; iter != iter_end; iter++)
      emplace_back(*iter);
    }
    return *this;
  }
  list(list&& that) {
    *this = std::move(that);
  }
  list& operator=(list&& that) {
    if (&that !=  this) {
      clear();
      swap(ptr_head_, that.ptr_head_);
      swap(ptr_tail_, that.ptr_tail_);
    }
    return *this;
  }

 private:
  class Node {
   public:
    value_type value;
    std::shared_ptr<Node> ptr_prev;
    std::shared_ptr<Node> ptr_next;

    template<class...Args>
    explicit Node(Args&&...args) : value(std::forward<Args>(args)...) { }
  };

 private:
  std::shared_ptr<Node> ptr_head_{ nullptr };
  std::shared_ptr<Node> ptr_tail_{ nullptr };

 public:
  class iterator : public abc::iterator<std::forward_iterator_tag, value_type> {
    friend list;

   protected:
    Node* ptr_node{nullptr};

   public:
    explicit iterator(Node* ptr) : ptr_node(ptr) { }
    iterator(iterator const& that) { *this = that; }
    iterator& operator=(iterator const& that) {
      this->ptr_node = that.ptr_node;
      return *this;
    }
    iterator(iterator&& that) { *this = std::move(that); }
    iterator& operator=(iterator&& that) {
      this->ptr_node = that.ptr_node;
      that.ptr_node = NULL;
      return *this;
    }

    value_type& operator*() const { return ptr_node->value; }
    value_type* operator->() const { return &(this->operator*()); }

    bool operator==(const iterator& that) const {
      return ptr_node == that.ptr_node;
    }
    bool operator!=(const iterator& that) const {
      return !(operator==(that));
    }

    iterator operator+(int n) {
      Node* node_a = ptr_node;
      while (n != 0) {
        node_a = node_a->ptr_next.get();
        n--;
      }
      return iterator(node_a);
    }
    iterator operator-(int n) {
      auto node_a = ptr_node;
      while (n != 0) {
        node_a = node_a->ptr_prev.get();
        n--;
      }
      return iterator(node_a);
    }

    iterator operator++(int) {  // i++
      auto iter = iterator(ptr_node);
      ptr_node = ptr_node->ptr_next.get();
      return iter;
    }
    iterator& operator++() {  // ++i
      ptr_node = ptr_node->ptr_next.get();
      return *this;
    }
    iterator operator--(int) {  // i--
      auto iter = iterator(ptr_node);
      ptr_node = ptr_node->ptr_prev.get();
      return iter;
    }
    iterator& operator--() {  // --i
      ptr_node = ptr_node->ptr_prev.get();
      return *this;
    }
  };

  iterator begin() { return iterator(ptr_head_.get()); }
  iterator end() { return iterator(nullptr); }
  iterator rbegin() { return iterator(ptr_tail_.get()); }
  iterator rend() { return iterator(nullptr); }

  class const_iterator : public iterator {
    friend list;
   public:
    explicit const_iterator(Node* ptr) : iterator(ptr) { }

    const value_type& operator*() {return ptr_node->value;}
    const value_type* operator->() {return &(this->operator*());}
  };

  const_iterator cbegin()  const { return  const_iterator(ptr_head_.get()); }
  const_iterator cend()  const { return const_iterator(nullptr); }
  const_iterator crbegin() const { return const_iterator(ptr_tail_.get()); }
  const_iterator crend() const { return const_iterator(nullptr); }

  const_iterator begin() const  { return cbegin(); }
  const_iterator end() const  { return cend(); }
  const_iterator rbegin() const { return crbegin(); }
  const_iterator rend() const { return crend(); }

 public:
  bool empty() {
    return (ptr_head_ == nullptr && ptr_tail_ == nullptr);
  }

  size_type size() {
    auto iter = this->begin();
    size_type n = 0;
    while (iter != this->end()) {
      n++;
      iter++;
    }
    return n;
  }

 public:
  template <class...Args>
  void emplace_back(Args&&...args) {
    std::shared_ptr<Node> ptr_new = std::make_shared<Node>(
      std::forward<Args>(args)...);
    if (ptr_tail_ == nullptr) {
      ptr_tail_ = ptr_new;
      ptr_head_ = ptr_new;
    } else {
      ptr_tail_->ptr_next = ptr_new;
      ptr_new->ptr_prev = ptr_tail_;
      ptr_tail_ = ptr_new;
    }
  }

  value_type& back() { return ptr_tail_->value; }
  value_type& front() { return ptr_head_->value; }

  template <class... Args>
  void emplace_front(Args&&... args) {
    auto ptr_new = std::make_shared<Node>(std::forward<Args>(args)...);
    if (ptr_head_) {
      assert(ptr_tail_ != nullptr);
      ptr_head_->ptr_prev = ptr_new;
      assert(ptr_new.use_count() == 2);
      ptr_new->ptr_next = ptr_head_;
      ptr_head_ = ptr_new;
      assert(ptr_head_ != ptr_tail_);
    } else {
      assert(ptr_tail_ == nullptr);
      assert(ptr_new.use_count() == 1);
      ptr_head_ = ptr_new;
      assert(ptr_new.use_count() == 2);
      ptr_tail_ = ptr_new;
      assert(ptr_new.use_count() == 3);
      assert(ptr_head_ == ptr_tail_);
    }
    assert(ptr_head_ != nullptr);
    assert(ptr_tail_ != nullptr);
  }

  void pop_front() noexcept {
    ptr_head_ = ptr_head_->ptr_next;
    if (ptr_head_) {
      assert(ptr_tail_);
      ptr_head_->ptr_prev.reset();
    } else {
      ptr_tail_.reset();
    }
  }
  void pop_back() noexcept {
    ptr_tail_ = ptr_tail_->ptr_prev;
    if (ptr_tail_) {
      assert(ptr_head_);
      ptr_tail_->ptr_next.reset();
    } else {
      ptr_head_.reset();
    }
  }

  void clear() noexcept {
    while (ptr_head_) { pop_front(); }
    assert(ptr_head_ == nullptr);
    assert(ptr_tail_ == nullptr);
  }

  iterator erase(iterator iter) {
    if (iter == this->end()) return iter;
    else {
      if (iter+1 == this->end()) {
        this->pop_back();
        return this->end();
      } else if (iter-1 == this->end()) {
        this->pop_front();
        return iterator(this->ptr_head_.get());
      } else {
        auto iter_l = iter-1;
        auto iter_r = iter+1;
        iter_l.ptr_node->ptr_next = iter.ptr_node->ptr_next;
        iter_r.ptr_node->ptr_prev = iter.ptr_node->ptr_prev;
        return iter_r;
      }
    }
  }

  template <class...Args>
  iterator emplace_after(iterator iter, Args&&...args) {
    if (iter.ptr_node == nullptr) {
      emplace_back(std::forward<Args>(args)...);
      iter = this->begin();
    } else {
      if (iter.ptr_node->ptr_next == nullptr) {
        emplace_back(std::forward<Args>(args)...);
        iter++;
      } else {
        auto ptr_new = std::make_shared<Node>(std::forward<Args>(args)...);
        iterator iter0 = iter++;
        iter0.ptr_node->ptr_next = ptr_new;
        assert(iter.ptr_node);
        iter.ptr_node->ptr_prev = ptr_new;

        if (iter0.ptr_node->ptr_prev == nullptr)
          ptr_new->ptr_prev = ptr_head_;
        else
          ptr_new->ptr_prev = (iter0-1).ptr_node->ptr_next;
        if (iter.ptr_node->ptr_next == nullptr)
          ptr_new->ptr_next = ptr_tail_;
        else
          ptr_new->ptr_next = (iter+1).ptr_node->ptr_prev;
        iter--;
      }
    }
    return iter;
  }

  template <class...Args>
  iterator emplace_before(iterator iter, Args&&...args) {
    if (iter.ptr_node == nullptr) {
      emplace_back(std::forward<Args>(args)...);
      iter = this->begin();
    } else {
      if (iter.ptr_node->ptr_prev == nullptr) {
        emplace_front(std::forward<Args>(args)...);
        iter--;
      } else {
        std::shared_ptr<Node> ptr_new = std::make_shared<Node>(
          std::forward<Args>(args)...);
        iterator iter0 = iter--;
        iter0.ptr_node->ptr_prev = ptr_new;
        iter.ptr_node->ptr_next = ptr_new;
        if (iter0.ptr_node == ptr_tail_.get())
          ptr_new->ptr_next = ptr_tail_;
        else
          ptr_new->ptr_next = ((iter0+1).ptr_node)->ptr_prev;
        if (iter.ptr_node == ptr_head_.get())
          ptr_new->ptr_prev = ptr_head_;
        else
          ptr_new->ptr_prev = ((iter-1).ptr_node)->ptr_next;
        iter++;
      }
    }
    return iter;
  }
};

template <class value_type>
bool operator==(const list<value_type>& lhs, const list<value_type>& rhs) {
  auto iter1 = lhs.begin();
  auto iter2 = rhs.begin();
  while (iter1 != lhs.end() && iter2 != rhs.end()) {
    if (*iter1 !=  *iter2) return false;
    iter1++;
    iter2++;
  }
  if (iter1 == lhs.end() && iter2 == rhs.end()) return true;
  else return false;
}

template <class value_type>
bool operator!=(const list<value_type>& lhs, const list<value_type>& rhs) {
  return !(operator==(lhs, rhs));
}

}  // namespace abc

#endif  // ABC_LIST_H_
