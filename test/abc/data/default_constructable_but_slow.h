// Copyright 2020 Weicheng Pei

#ifndef TEST_ABC_DATA_DEFAULT_CONSTRUCTABLE_BUT_SLOW_H_
#define TEST_ABC_DATA_DEFAULT_CONSTRUCTABLE_BUT_SLOW_H_

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

#endif  // TEST_ABC_DATA_DEFAULT_CONSTRUCTABLE_BUT_SLOW_H_
