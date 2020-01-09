// Copyright 2020 Weicheng Pei

#ifndef TEST_ABC_DATA_COPYABLE_H_
#define TEST_ABC_DATA_COPYABLE_H_

#include <memory>

#include "abc/data/default_constructable_but_slow.h"

namespace abc {
namespace data {

class Copyable : public DefaultConstructableButSlow {
 private:
  std::shared_ptr<int> id_;  // copy-able data member

 public:
  // Accessor:
  int Id() const { return id_ ? *id_ : -1; }
  // Constructors:
  Copyable() : DefaultConstructableButSlow() {}
  explicit Copyable(int i) : id_(std::make_shared<int>(i)) {}
  // Destructor:
  ~Copyable() noexcept = default;
  // Copy operations:
  Copyable(Copyable const& other) = default;
  Copyable& operator=(Copyable const& other) = default;
  // The implicit move operations are suppressed by the declarations above.
  // Comparing operations:
  bool operator==(const Copyable& that) const { return Id() == that.Id(); }
  bool operator!=(const Copyable& that) const { return !operator==(that); }
};  // class Copyable

}  // namespace data
}  // namespace abc

#endif  // TEST_ABC_DATA_COPYABLE_H_
