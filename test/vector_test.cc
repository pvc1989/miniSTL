// Copyright 2019 Weicheng Pei

#include "vector.h"

#include <algorithm>
#include <forward_list>
#include <vector>

#include "gtest/gtest.h"

class VectorTest : public ::testing::Test {
 protected:
  // helper class
  struct Kitten {
    int id;
    explicit Kitten(int id) : id(id) { }
    Kitten() = default;
    ~Kitten() noexcept = default;
    Kitten(const Kitten&) = default;
    Kitten& operator=(const Kitten&) = default;
    Kitten(Kitten&&) noexcept = default;
    Kitten& operator=(Kitten&&) noexcept = default;
    bool operator==(const Kitten& that) const { return id == that.id; }
    bool operator!=(const Kitten& that) const { return id != that.id; }
  };
  // common data
  std::forward_list<int> std_vector_of_id{ 4, 3, 2, 1 };
  std::vector<Kitten> std_vector_of_kitten;
  pvc::vector<Kitten> pvc_vector_of_kitten;
};
TEST_F(VectorTest, Construction_1) {
  pvc_vector_of_kitten = pvc::vector<Kitten>(4);
  std_vector_of_kitten = std::vector<Kitten>(4);
  EXPECT_EQ(pvc_vector_of_kitten.size(), std_vector_of_kitten.size());
  EXPECT_EQ(pvc_vector_of_kitten.capacity(), std_vector_of_kitten.capacity());
  auto pvc_vector = pvc::vector<Kitten>(4);
  auto std_vector = std::vector<Kitten>(4);
  EXPECT_EQ(pvc_vector.size(), std_vector.size());
  EXPECT_EQ(pvc_vector.capacity(), std_vector.capacity());
}
TEST_F(VectorTest, Construction_2) {
  auto kitten = Kitten(9);
  auto size = 4;
  pvc_vector_of_kitten = pvc::vector<Kitten>(size, kitten);
  std_vector_of_kitten = std::vector<Kitten>(size, kitten);
  EXPECT_EQ(pvc_vector_of_kitten.size(), std_vector_of_kitten.size());
  EXPECT_EQ(pvc_vector_of_kitten.capacity(), std_vector_of_kitten.capacity());
  auto pvc_vector = pvc::vector<Kitten>(size, kitten);
  auto std_vector = std::vector<Kitten>(size, kitten);
  EXPECT_EQ(pvc_vector.size(), std_vector.size());
  for (int i = 0; i < size; i++) {
    EXPECT_EQ(pvc_vector[i], std_vector[i]);
  }
}
TEST_F(VectorTest, Construction_3) {
  auto list = {1, 2, 3, 5, 6};
  auto pvc_vector = pvc::vector<int>(list);
  auto std_vector = std::vector<int>(list);
  EXPECT_EQ(pvc_vector.size(), std_vector.size());
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(pvc_vector.at(i), std_vector.at(i));
  }
  list = {2, 4};
  pvc_vector = list;
  std_vector = list;
  EXPECT_EQ(pvc_vector.size(), std_vector.size());
  EXPECT_EQ(pvc_vector.capacity(), std_vector.capacity());
}
TEST_F(VectorTest, Empty) {
  EXPECT_EQ(pvc_vector_of_kitten.empty(), std_vector_of_kitten.empty());
}
TEST_F(VectorTest, EmplaceBack) {
  for (auto& i : std_vector_of_id) {
    std_vector_of_kitten.emplace_back(i);
    pvc_vector_of_kitten.emplace_back(i);
  }
  EXPECT_EQ(pvc_vector_of_kitten.empty(), std_vector_of_kitten.empty());
}
TEST_F(VectorTest, Resize) {
  auto kitten = Kitten(9);
  for (auto& i : std_vector_of_id) {
    std_vector_of_kitten.emplace_back(i);
    pvc_vector_of_kitten.emplace_back(i);
  }
  std_vector_of_kitten.resize(8, kitten);
  pvc_vector_of_kitten.resize(8, kitten);
  EXPECT_EQ(pvc_vector_of_kitten.size(), std_vector_of_kitten.size());
  EXPECT_EQ(pvc_vector_of_kitten.capacity(), std_vector_of_kitten.capacity());
  std_vector_of_kitten.resize(2);
  pvc_vector_of_kitten.resize(2);
  EXPECT_EQ(pvc_vector_of_kitten.size(), std_vector_of_kitten.size());
  EXPECT_EQ(pvc_vector_of_kitten.capacity(), std_vector_of_kitten.capacity());
}
TEST_F(VectorTest, PushBack) {
  auto kitten = Kitten(9);
  for (auto& i : std_vector_of_id) {
    std_vector_of_kitten.push_back(kitten);
    pvc_vector_of_kitten.push_back(kitten);
  }
  EXPECT_EQ(pvc_vector_of_kitten.empty(), std_vector_of_kitten.empty());
}
TEST_F(VectorTest, Size_and_Capacity) {
  for (const auto& i : std_vector_of_id) {
    std_vector_of_kitten.emplace_back(i);
    pvc_vector_of_kitten.emplace_back(i);
    EXPECT_EQ(pvc_vector_of_kitten.size(), std_vector_of_kitten.size());
    EXPECT_EQ(pvc_vector_of_kitten.capacity(), std_vector_of_kitten.capacity());
  }
}
TEST_F(VectorTest, Front) {
  for (const auto& i : std_vector_of_id) {
    std_vector_of_kitten.emplace_back(i);
    pvc_vector_of_kitten.emplace_back(i);
    EXPECT_EQ(pvc_vector_of_kitten.front(), std_vector_of_kitten.front());
  }
}
TEST_F(VectorTest, Back) {
  for (const auto& i : std_vector_of_id) {
    std_vector_of_kitten.emplace_back(i);
    pvc_vector_of_kitten.emplace_back(i);
    EXPECT_EQ(pvc_vector_of_kitten.back(), std_vector_of_kitten.back());
  }
}
TEST_F(VectorTest, PopBack) {
  for (const auto& i : std_vector_of_id) {
    std_vector_of_kitten.emplace_back(i);
    pvc_vector_of_kitten.emplace_back(i);
  }
  while (!std_vector_of_kitten.empty()) {
    std_vector_of_kitten.pop_back();
    pvc_vector_of_kitten.pop_back();
    EXPECT_EQ(pvc_vector_of_kitten.empty(), std_vector_of_kitten.empty());
  }
}
TEST_F(VectorTest, At) {
  int j = 0;
  for (const auto& i : std_vector_of_id) {
    std_vector_of_kitten.emplace_back(i);
    pvc_vector_of_kitten.emplace_back(i);
    EXPECT_EQ(pvc_vector_of_kitten.at(j), std_vector_of_kitten.at(j));
    j++;
  }
}
TEST_F(VectorTest, operator) {
  int j = 0;
  for (const auto& i : std_vector_of_id) {
    std_vector_of_kitten.emplace_back(i);
    pvc_vector_of_kitten.emplace_back(i);
    EXPECT_EQ(pvc_vector_of_kitten[j], std_vector_of_kitten[j]);
    j++;
  }
}
TEST_F(VectorTest, Iterator) {
  for (const auto& i : std_vector_of_id) {
    pvc_vector_of_kitten.emplace_back(i);
  }
  // look for an object in the vector
  auto iter = std::find(
      pvc_vector_of_kitten.begin(),
      pvc_vector_of_kitten.end(),
      Kitten(2));
  EXPECT_NE(iter, pvc_vector_of_kitten.end());
  EXPECT_EQ(*iter, Kitten(2));
  // look for an object not in the vector
  iter = std::find(
      pvc_vector_of_kitten.begin(),
      pvc_vector_of_kitten.end(),
      Kitten(-2));
  EXPECT_EQ(iter, pvc_vector_of_kitten.end());
}
TEST_F(VectorTest, Equal) {
  auto new_vector_of_kitten = decltype(pvc_vector_of_kitten)();
  for (const auto& i : std_vector_of_id) {
    pvc_vector_of_kitten.emplace_back(i);
    new_vector_of_kitten.emplace_back(i);
  }
  EXPECT_TRUE(pvc_vector_of_kitten == pvc_vector_of_kitten);
  EXPECT_TRUE(new_vector_of_kitten == pvc_vector_of_kitten);
  EXPECT_FALSE(pvc_vector_of_kitten != pvc_vector_of_kitten);
  EXPECT_FALSE(new_vector_of_kitten != pvc_vector_of_kitten);
}
TEST_F(VectorTest, Swap) {
  auto list_a = {1, 2, 3, 4};
  auto list_b = {5, 6};
  auto a = pvc::vector<int>(list_a);
  auto b = pvc::vector<int>(list_b);
  auto size_a = a.size();
  auto size_b = b.size();
  auto end_of_a = a.back();
  auto end_of_b = b.back();
  a.swap(b);
  EXPECT_EQ(a.size(), size_b);
  EXPECT_EQ(a.capacity(), size_b);
  EXPECT_EQ(a.back(), end_of_b);
  EXPECT_EQ(b.size(), size_a);
  EXPECT_EQ(b.capacity(), size_a);
  EXPECT_EQ(b.back(), end_of_a);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
