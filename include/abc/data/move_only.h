#ifndef ABC_DATA_MOVE_ONLY_H_
#define ABC_DATA_MOVE_ONLY_H_

#include <memory>

#include "abc/data/default_constructable_but_slow.h"

namespace abc {
namespace data {

class MoveOnly : public DefaultConstructableButSlow {
 public:
  std::unique_ptr<int> id;  // move-only data member
  // Constructors:
  MoveOnly() : DefaultConstructableButSlow() {}
  MoveOnly(int i) : id(std::make_unique<int>(i)) {}
  // Destructor:
  ~MoveOnly() noexcept = default;
  // Move operations:
  MoveOnly(MoveOnly&& other) = default;
  MoveOnly& operator=(MoveOnly&& other) = default;
  // Copy operations are deleted:
  MoveOnly(MoveOnly const& other) = delete;
  MoveOnly& operator=(MoveOnly const& other) = delete;
  // Comparing operations:
  bool operator==(const MoveOnly& that) const { return *id == *that.id; }
  bool operator!=(const MoveOnly& that) const { return *id != *that.id; }
};  // class MoveOnly

}  // namespace data
}  // namespace abc

#endif  // ABC_DATA_MOVE_ONLY_H_
