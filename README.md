# miniSTL

## Intention
This repo is a minimum implementation of the C++ *Standard Template Library (STL)*.
The intention is not to challenge those industrial-strength implementations, but to familiarize myself with [modern C++ (i.e. C++11/14/17)](https://en.cppreference.com/w/cpp/compiler_support) and its standard library (e.g. [containers](https://en.cppreference.com/w/cpp/container), [algorithms](https://en.cppreference.com/w/cpp/algorithm) and [utilities](https://en.cppreference.com/w/cpp/utility#General-purpose_utilities)).

## Build
```shell
git clone https://github.com/pvcStillInGradSchool/miniSTL.git
cd miniSTL
mkdir build
cd build
cmake -S .. -B .  # cmake 3.13.5+
cmake --build .
cd test
./forward_list_test
```
