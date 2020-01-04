// Copyright 2020 Weicheng Pei

#ifndef ABC_DATA_COPYABLE_H_
#define ABC_DATA_COPYABLE_H_

#include <memory>

#include "abc/data/default_constructable_but_slow.h"

namespace abc {
namespace data {

class Copyable : public DefaultConstructableButSlow {
 public:
  std::shared_ptr<int> id;  // copy-able data member
  // Constructors:
  Copyable() : DefaultConstructableButSlow() {}
  Copyable(int i) : id(std::make_shared<int>(i)) {}
  // Destructor:
  ~Copyable() noexcept = default;
  // Copy operations:
  Copyable(Copyable const& other) = default;
  Copyable& operator=(Copyable const& other) = default;
  // The implicit move operations are suppressed by the declarations above.
  // Comparing operations:
  bool operator==(const Copyable& that) const { return *id == *that.id; }
  bool operator!=(const Copyable& that) const { return *id != *that.id; }
};  // class Copyable

}  // namespace data
}  // namespace abc

#endif  // ABC_DATA_COPYABLE_H_
