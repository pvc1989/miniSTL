// Copyright 2019 Weicheng Pei
// #define PVC_USE_SMART_POINTER_
#include "forward_list.h"

#include <algorithm>
#include <chrono>
#include <forward_list>

#include "gtest/gtest.h"

class ForwardListTest : public ::testing::Test {
 protected:
  // helper class
  struct Kitten {
    int id;
    explicit Kitten(int id) : id(id) { }
    ~Kitten() noexcept = default;
    Kitten(const Kitten&) = default;
    Kitten& operator=(const Kitten&) = default;
    Kitten(Kitten&&) noexcept = default;
    Kitten& operator=(Kitten&&) noexcept = default;
    bool operator==(const Kitten& that) const { return id == that.id; }
    bool operator!=(const Kitten& that) const { return id != that.id; }
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

TEST_F(ForwardListTest, Front) {
  for (const auto& i : std_list_of_id) {
    std_list_of_kitten.emplace_front(i);
    pvc_list_of_kitten.emplace_front(i);
    EXPECT_EQ(pvc_list_of_kitten.front(), std_list_of_kitten.front());
  }
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

TEST_F(ForwardListTest, Iterator) {
  for (const auto& i : std_list_of_id) {
    pvc_list_of_kitten.emplace_front(i);
  }
  // look for an object in the list
  auto iter = std::find(
      pvc_list_of_kitten.begin(),
      pvc_list_of_kitten.end(),
      Kitten(2));
  EXPECT_NE(iter, pvc_list_of_kitten.end());
  EXPECT_EQ(*iter, Kitten(2));
  // look for an object not in the list
  iter = std::find(
      pvc_list_of_kitten.begin(),
      pvc_list_of_kitten.end(),
      Kitten(-2));
  EXPECT_EQ(iter, pvc_list_of_kitten.end());
}

TEST_F(ForwardListTest, EmplaceAfter) {
  for (const auto& i : std_list_of_id) {
    std_list_of_kitten.emplace_front(i);
    pvc_list_of_kitten.emplace_front(i);
  }
  const auto& target = Kitten(2);
  auto std_iter = std::find(
      std_list_of_kitten.begin(),
      std_list_of_kitten.end(),
      target);
  std_iter = std_list_of_kitten.emplace_after(std_iter, 0);
  auto pvc_iter = std::find(
      pvc_list_of_kitten.begin(),
      pvc_list_of_kitten.end(),
      target);
  pvc_iter = pvc_list_of_kitten.emplace_after(pvc_iter, 0);
  EXPECT_EQ(*pvc_iter, *std_iter);
  while (!pvc_list_of_kitten.empty()) {
    EXPECT_EQ(pvc_list_of_kitten.front(), std_list_of_kitten.front());
    pvc_list_of_kitten.pop_front();
    std_list_of_kitten.pop_front();
  }
}

TEST_F(ForwardListTest, Equal) {
  auto new_list_of_kitten = decltype(pvc_list_of_kitten)();
  for (const auto& i : std_list_of_id) {
    pvc_list_of_kitten.emplace_front(i);
    new_list_of_kitten.emplace_front(i);
  }
  EXPECT_TRUE(pvc_list_of_kitten == pvc_list_of_kitten);
  EXPECT_TRUE(new_list_of_kitten == pvc_list_of_kitten);
  EXPECT_FALSE(pvc_list_of_kitten != pvc_list_of_kitten);
  EXPECT_FALSE(new_list_of_kitten != pvc_list_of_kitten);
}

TEST_F(ForwardListTest, Copy) {
  for (const auto& i : std_list_of_id) {
    pvc_list_of_kitten.emplace_front(i);
  }
  // test copy constructor:
  auto new_list_of_kitten = pvc_list_of_kitten;
  EXPECT_EQ(new_list_of_kitten, pvc_list_of_kitten);
  // test copy assignment operator:
  new_list_of_kitten = pvc_list_of_kitten;
  EXPECT_EQ(new_list_of_kitten, pvc_list_of_kitten);
  // test self assignment:
  new_list_of_kitten = new_list_of_kitten;
  EXPECT_EQ(new_list_of_kitten, pvc_list_of_kitten);
}

TEST_F(ForwardListTest, Move) {
  for (const auto& i : std_list_of_id) {
    pvc_list_of_kitten.emplace_front(i);
  }
  // test move constructor:
  auto copied_list_of_kitten = pvc_list_of_kitten;
  auto moved_list_of_kitten = pvc::move(pvc_list_of_kitten);
  EXPECT_EQ(moved_list_of_kitten, copied_list_of_kitten);
  // test move assignment operator:
  pvc_list_of_kitten = pvc::move(copied_list_of_kitten);
  EXPECT_EQ(moved_list_of_kitten, pvc_list_of_kitten);
  // test self assignment:
  moved_list_of_kitten = pvc::move(moved_list_of_kitten);
  EXPECT_EQ(moved_list_of_kitten, pvc_list_of_kitten);
}

TEST_F(ForwardListTest, Performance) {
  using clock = std::chrono::high_resolution_clock;
  int n = 1000000;
  auto ticks = [n](auto& list) {
    auto start = clock::now();
    for (int i = 0; i != n; ++i) {
      list.emplace_front(i);
    }
    list.clear();
    std::chrono::duration<double> duration = clock::now() - start;
    return duration.count();
  };
  auto t_std = ticks(std_list_of_kitten);
  auto t_pvc = ticks(pvc_list_of_kitten);
  EXPECT_LT(t_pvc/t_std, 1.2);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
