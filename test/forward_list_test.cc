// Copyright 2019 Weicheng Pei

#include "forward_list.h"

#include <algorithm>
#include <iostream>
#include <forward_list>

#include "gtest/gtest.h"

using std::cout;
using std::endl;

class ForwardListTest : public ::testing::Test {
 protected:
  // helper class
  struct Kitten {
    const int id;
    explicit Kitten(int id) : id(id) { cout << "Kitten(" << id << ')' << endl; }
    ~Kitten() { cout << id << " ~Kitten()" << endl; }
    Kitten(const Kitten&) = default;
    Kitten& operator=(const Kitten&) = default;
    Kitten(Kitten&&) noexcept = default;
    Kitten& operator=(Kitten&&) noexcept = default;
  };
  // common data
  std::forward_list<int> std_list_of_id{ 4, 3, 2, 1 };
  std::forward_list<Kitten> std_list_of_kitten;
  pvc::forward_list<Kitten> pvc_list_of_kitten;
};

TEST_F(ForwardListTest, Empty) {
  EXPECT_EQ(pvc_list_of_kitten.empty(), std_list_of_kitten.empty());
}

TEST_F(ForwardListTest, EmplaceFront) {
  for (const auto& i : std_list_of_id) {
    std_list_of_kitten.emplace_front(i);
    pvc_list_of_kitten.emplace_front(i);
  }
  EXPECT_EQ(pvc_list_of_kitten.empty(), std_list_of_kitten.empty());
}

TEST_F(ForwardListTest, ListInitialize) {
  FAIL();
}

TEST_F(ForwardListTest, PopFront) {
  for (const auto& i : std_list_of_id) {
    std_list_of_kitten.emplace_front(i);
    pvc_list_of_kitten.emplace_front(i);
  }
  while (!std_list_of_kitten.empty()) {
    std_list_of_kitten.pop_front();
    pvc_list_of_kitten.pop_front();
    EXPECT_EQ(pvc_list_of_kitten.empty(), std_list_of_kitten.empty());
  }
}

TEST_F(ForwardListTest, BeginEnd) {
  FAIL();
}

TEST_F(ForwardListTest, DoAfter) {
  FAIL();
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
