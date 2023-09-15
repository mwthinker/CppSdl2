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

## Usage
When using CMake, in CMakeLists.txt

```cmake
include(FetchContent)
# Load external github projects
FetchContent_Declare(CppSdl2
    GIT_REPOSITORY
        https://github.com/mwthinker/CppSdl2.git
    OVERRIDE_FIND_PACKAGE
)

# ... some code
find_package(CppSdl2 REQUIRED)

# ... some code
target_link_libraries(<YourProject>
    PRIVATE
        CppSdl2
)
```

In code main.cpp

```cpp
#include <sdl/imguiwindow.h>

class ExampleWindow : public sdl::ImGuiWindow {
public:
    ExampleWindow() = default;

private:
    void imGuiUpdate(const sdl::DeltaTime& deltaTime) override {
        ImGui::MainWindow("ExampleWindow", [this](){
            if (ImGui::Button("Quit")) {
                quit();
            }
        });
    }

    void imGuiEventUpdate(const SDL_Event& windowEvent) override {
        switch (windowEvent.type) {
            case SDL_WINDOWEVENT:
                switch (windowEvent.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:
                        quit();
                        break;
                }
                break;
            case SDL_KEYDOWN:
                switch (windowEvent.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit();
                        break;
                }
                break;
            case SDL_QUIT:
                quit();
                break;
        }
    }

    std::vector<std::unique_ptr<mwetris::ui::SubWindow>> subWindows_;
    std::shared_ptr<mwetris::game::DeviceManager> deviceManager_;
    std::shared_ptr<mwetris::network::DebugServer> debugServer_;
};

int main() {
    ExampleWindow exampleWindow;
    exampleWindow.startLoop();
    return 0;
}

```

## Open source
The code is licensed under the MIT License (see LICENSE.txt).
