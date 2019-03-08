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

 private:
  std::unique_ptr<Node> uptr_head_{ nullptr };
};
}  // namespace pvc
#endif  // PVC_FORWARD_LIST_H_
