#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include "batchtriangles.h"

#include <sdl/window.h>
#include <sdl/sprite.h>
#include <sdl/gamecontroller.h>

#include <functional>
#include <vector>

class TestWindow : public sdl::Window {
public:
	TestWindow(const sdl::Sprite& sprite = {});

	void setSpaceFunction(std::function<void()>&& onSpacePressed) {
		onSpacePressed_ = std::move(onSpacePressed);
	}

private:
	void initPreLoop() override;

    void update(const sdl::DeltaTime& deltaTime) override;

	void eventUpdate(const SDL_Event& windowEvent) override;

	void resize(int w, int h);

	void removeGamepad(SDL_JoystickID instanceId);

	bool focus_ = true;
	sdl::Sprite sprite_;
	sdl::Sprite text_;
	int controllerEvent_ = 0;
	
	std::function<void()> onSpacePressed_;

	std::shared_ptr<TestShader> shader_;
	std::shared_ptr<BatchTriangles> batch_;
	std::shared_ptr<BatchTriangles> batch2_;
	
	std::vector<sdl::GameController> gameControllers_;
};

#endif
