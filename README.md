# sagan

Yet another attempt to build a space game based on EnTT

## Requirements
* [Python 3.x](https://python.org)
* [Conan](https://conan.io) can be installed via: `pip install --user conan==1.60.0
**  conan 2.x.x is currently not in the scope`

## Build
```
git clone https://github.com/psyinf/sagan.git
cd sagan
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```
The CMake configure step should download all dependencies either as binary or as source code and start to build them. 

