// Copyright 2020 Weicheng Pei

#ifndef TEST_ABC_DATA_MOVE_ONLY_H_
#define TEST_ABC_DATA_MOVE_ONLY_H_

#include <memory>

#include "abc/data/default_constructable_but_slow.h"

namespace abc {
namespace data {

class MoveOnly : public DefaultConstructableButSlow {
 private:
  std::unique_ptr<int> id_;  // move-only data member

 public:
  // Accessor:
  int Id() const { return id_ ? *id_ : -1; }
  // Constructors:
  MoveOnly() : DefaultConstructableButSlow() {}
  explicit MoveOnly(int i) : id_(std::make_unique<int>(i)) {}
  // Destructor:
  ~MoveOnly() noexcept = default;
  // Move operations:
  MoveOnly(MoveOnly&& other) = default;
  MoveOnly& operator=(MoveOnly&& other) = default;
  // Copy operations are deleted:
  MoveOnly(MoveOnly const& other) = delete;
  MoveOnly& operator=(MoveOnly const& other) = delete;
  // Comparing operations:
  bool operator==(const MoveOnly& that) const { return Id() == that.Id(); }
  bool operator!=(const MoveOnly& that) const { return !operator==(that); }
};  // class MoveOnly

}  // namespace data
}  // namespace abc

#endif  // TEST_ABC_DATA_MOVE_ONLY_H_
