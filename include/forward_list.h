// Copyright 2019 Weicheng Pei

#ifndef PVC_FORWARD_LIST_H_
#define PVC_FORWARD_LIST_H_

#include <memory>
#include <utility>

namespace pvc {
template <class T>
class forward_list {
 public:
  using value_type = T;
  using size_type = std::size_t;
  using reference = value_type&;

 private:
  struct Node {
    value_type value;
    std::unique_ptr<Node> uptr_next;

    template <class... Args>
    explicit Node(Args... args) : value(std::forward<Args>(args)...) { }
  };

   private:
    Node* ptr_node{ nullptr };
  };

 public:  // non-modifying methods
  bool empty() const noexcept { return !uptr_head_.get(); }

 public:  // modifying methods
  template <class... Args>
  void emplace_front(Args&&... args) {
    auto uptr_new = std::make_unique<Node>(std::forward<Args>(args)...);
    uptr_new->uptr_next.reset(uptr_head_.release());
    uptr_new.swap(uptr_head_);
  }
  
  void pop_front() {
    std::unique_ptr<Node> uptr_next(uptr_head_->uptr_next.release());
    uptr_head_.swap(uptr_next);
  }

 private:
  std::unique_ptr<Node> uptr_head_{ nullptr };
};
}  // namespace pvc
#endif  // PVC_FORWARD_LIST_H_
