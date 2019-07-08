#ifndef TESTIMGUIWINDOW_H
#define TESTIMGUIWINDOW_H

#include "testshader.h"
#include "batchtriangles.h"

#include <sdl/imguiwindow.h>
#include <sdl/sprite.h>
#include <sdl/text.h>
#include <sdl/gamecontroller.h>

#include <functional>
#include <vector>

class TestImGuiWindow : public sdl::ImGuiWindow {
public:
	TestImGuiWindow();

private:
	void initPreLoop() override;

	void initOpenGl() override;

    void update(double deltaTime) override;

	void eventUpdate(const SDL_Event& windowEvent) override;
};

#endif // TESTIMGUIWINDOW_H
