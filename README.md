# miniSTL

## Intention
This repo is a minimum implementation of the C++ standard template library.
The intention is not to challenge those industrial-strength implementations, but to familiarize myself with modern C++ (i.e. C++11/14/17) and its standard library (e.g. containers, algorithms).

## Build
```shell
git clone https://github.com/pvcStillInGradSchool/miniSTL.git
cd miniSTL
mkdir build
cd build
cmake -S .. -B .
cmake --build .
cd test
./forward_list_test
```
