#ifndef ABC_DATA_COPY_ONLY_H_
#define ABC_DATA_COPY_ONLY_H_

#include <memory>

#include "abc/data/default_constructable_but_slow.h"

namespace abc {
namespace data {

class CopyOnly : public DefaultConstructableButSlow {
 public:
  std::shared_ptr<int> id;  // copy-able data member
  // Constructors:
  CopyOnly() : DefaultConstructableButSlow() {}
  CopyOnly(int i) : id(std::make_shared<int>(i)) {}
  // Destructor:
  ~CopyOnly() noexcept = default;
  // Copy operations:
  CopyOnly(CopyOnly const& other) = default;
  CopyOnly& operator=(CopyOnly const& other) = default;
  // Move operations are deleted:
  CopyOnly(CopyOnly&& other) = default;
  CopyOnly& operator=(CopyOnly&& other) = default;
  // Comparing operations:
  bool operator==(const CopyOnly& that) const { return *id == *that.id; }
  bool operator!=(const CopyOnly& that) const { return *id != *that.id; }
};  // class CopyOnly

}  // namespace data
}  // namespace abc

#endif  // ABC_DATA_COPY_ONLY_H_
