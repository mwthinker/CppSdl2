# sdl::CppSdl2 [![CI build](https://github.com/mwthinker/CppSdl2/actions/workflows/ci.yml/badge.svg)](https://github.com/mwthinker/CppSdl2/actions/workflows/ci.yml)
A C++ wrapper around SDL2 (Simple Direct Media Layer) and OpenGl. Uses cmake.

## Developer environment
This project is developed using [vcpkg](https://github.com/microsoft/vcpkg.git) and [CMake](https://cmake.org/).

```
git clone https://github.com/microsoft/vcpkg.git
```
Run bootstrap-vcpkg script belonging to the platform you are on.
Store the path to vcpkg in the enviromental variable VCPKG_ROOT.

```
# (assuming in a unix host otherwise change to "windows")
git clone https://github.com/mwthinker/CppSdl2; cd CppSdl2
mkdir build
cd build
cmake --preset=unix -DCppSdl2_Example=1 -DCppSdl2_Test=1 ..
make
```
## Open source
The code is licensed under the MIT License (see LICENSE.txt).
