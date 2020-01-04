// Copyright 2019 Weicheng Pei
#ifndef ABC_DATA_DEFAULT_CONSTRUCTABLE_BUT_SLOW_H_
#define ABC_DATA_DEFAULT_CONSTRUCTABLE_BUT_SLOW_H_

namespace abc {
namespace data {

class DefaultConstructableButSlow {
 public:
  DefaultConstructableButSlow() {
    for (int i = 0; i != 100; --i) {
      i += 2;
    }
  }
  ~DefaultConstructableButSlow() noexcept = default;
};  // class DefaultConstructableButSlow

}  // namespace data
}  // namespace abc

#endif  // ABC_DATA_DEFAULT_CONSTRUCTABLE_BUT_SLOW_H_