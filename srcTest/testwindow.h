#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include "testshader.h"
#include "batchtriangles.h"

#include <sdl/window.h>
#include <sdl/sprite.h>
#include <sdl/text.h>

#include <functional>

class TestWindow : public sdl::Window {
public:
	TestWindow(sdl::Sprite sprite = sdl::Sprite());

	// Execute the function when space bar is pressed.
	void setSpaceFunction(const std::function<void()> func) {
		func_ = func;
	}

	void setCenteredSprite(const sdl::Sprite& sprite) {
		sprite2_ = sprite;
	}

private:
	void initPreLoop() override;

    void update(double deltaTime) override;

	void eventUpdate(const SDL_Event& windowEvent) override;

	void resize(int w, int h);

	bool focus_;
	sdl::Sprite sprite_;
	sdl::Sprite sprite2_;	
	int controllerEvent_;
	
	std::function<void()> func_;
	std::shared_ptr<BatchTriangles> batch_;

	std::shared_ptr<TestShader> shader_;
};

#endif // TESTWINDOW_H
