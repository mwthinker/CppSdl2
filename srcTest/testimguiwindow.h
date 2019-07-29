#ifndef TESTIMGUIWINDOW_H
#define TESTIMGUIWINDOW_H

#include "testshader.h"
#include "batchtriangles.h"
#include "batchtrianglesindexes.h"

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

    void imGuiPreUpdate(double deltaTime) override;

	void eventUpdate(const SDL_Event& windowEvent) override;

	void initBatchTriangles();
	void initBatchTrianglesIndexes();

    std::shared_ptr<TestShader2> shader_;
    std::shared_ptr<BatchTriangles> batch_;
	std::shared_ptr<BatchTrianglesIndexes> batchIndexes_;
};

#endif // TESTIMGUIWINDOW_H
