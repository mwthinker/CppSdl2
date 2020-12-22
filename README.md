# CppSdl2
A C++ wrapper around SDL2 (Simple Direct Media Layer) and OpenGl. Uses cmake.

## Vcpkg
This project is easiest used with vcpkg.

```
git clone https://github.com/microsoft/vcpkg.git
```
Run bootstrap-vcpkg script belonging to the platform you are on.
Store the path to vcpkg in the enviromental variable VCPKG_ROOT.

```
vcpkg install sdl2-image sdl2-mixer sdl2-ttf sdl2 glad spdlog glm catch2
```

## CMake
Install cmake. To create the build project eather use the cmake-gui or commandline.

```
git clone https://github.com/microsoft/CppSdl2
```

Command line using make
```
git clone https://github.com/microsoft/CppSdl2; cd CppSdl2
mkdir build; cd build
cmake -DCppSdl2_Example=1 -DCppSdl2_Test=1 ..
make
```

## Open source
The code is licensed under the MIT License (see LICENSE.txt).