#include "testwindow.h"

#include <sdl/sprite.h>
#include <sdl/window.h>
#include <sdl/opengl.h>
#include <sdl/sprite.h>
#include <sdl/gamecontroller.h>
#include <sdl/framebuffer.h>

#include <fmt/core.h>

namespace {

	void printGameControllerButton(Uint8 button) {
		switch (button) {
			case SDL_CONTROLLER_BUTTON_A:
				fmt::println("SDL_CONTROLLER_BUTTON_A");
				break;
			case SDL_CONTROLLER_BUTTON_B:
				fmt::println("SDL_CONTROLLER_BUTTON_B");
				break;
			case SDL_CONTROLLER_BUTTON_X:
				fmt::println("SDL_CONTROLLER_BUTTON_X");
				break;
			case SDL_CONTROLLER_BUTTON_Y:
				fmt::println("SDL_CONTROLLER_BUTTON_Y");
				break;
			case SDL_CONTROLLER_BUTTON_BACK:
				fmt::println("SDL_CONTROLLER_BUTTON_BACK");
				break;
			case SDL_CONTROLLER_BUTTON_GUIDE:
				fmt::println("SDL_CONTROLLER_BUTTON_GUIDE");
				break;
			case SDL_CONTROLLER_BUTTON_START:
				fmt::println("SDL_CONTROLLER_BUTTON_START");
				break;
			case SDL_CONTROLLER_BUTTON_LEFTSTICK:
				fmt::println("SDL_CONTROLLER_BUTTON_LEFTSTICK");
				break;
			case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
				fmt::println("SDL_CONTROLLER_BUTTON_RIGHTSTICK");
				break;
			case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
				fmt::println("SDL_CONTROLLER_BUTTON_LEFTSHOULDER");
				break;
			case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
				fmt::println("SDL_CONTROLLER_BUTTON_RIGHTSHOULDER");
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_UP:
				fmt::println("SDL_CONTROLLER_BUTTON_DPAD_UP");
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
				fmt::println("SDL_CONTROLLER_BUTTON_DPAD_DOWN");
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
				fmt::println("SDL_CONTROLLER_BUTTON_DPAD_LEFT");
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
				fmt::println("SDL_CONTROLLER_BUTTON_DPAD_RIGHT");
				break;
			case SDL_CONTROLLER_BUTTON_MAX:
				fmt::println("SDL_CONTROLLER_BUTTON_MAX");
				break;
		}
	}

}

TestWindow::TestWindow(const sdl::Sprite& sprite)
	: Window{3, 3}
	, sprite_{sprite} {
	
	sdl::Window::setSize(512, 512);
	sdl::Window::setTitle("Test");
	sdl::Window::setIcon("tetris.bmp");
}

void TestWindow::update(const sdl::DeltaTime& deltaTime) {
	gl::glEnable(gl::GL_BLEND);
	gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);

	// Update model matrix.
	shader_->useProgram();
	sprite_.bind();
	shader_->setModelMatrix(Mat44(1));
	shader_->setProjectionMatrix(Mat44(1));
	batch_->draw();

	text_.bind();
	batch2_->draw();

	gl::glDisable(gl::GL_BLEND);
}

void TestWindow::eventUpdate(const SDL_Event& windowEvent) {
	switch (windowEvent.type) {
		case SDL_QUIT:
			quit();
			break;
		case SDL_MOUSEWHEEL: {
			auto add = windowEvent.wheel.y * 5;
			auto [w, h] = getSize();
			fmt::println("{} add = {}", windowEvent.wheel.which, add);
			setSize(add + w, add + h);
			break;
		}
		case SDL_WINDOWEVENT:
			switch (windowEvent.window.event) {
				case SDL_WINDOWEVENT_CLOSE:
					if (windowEvent.window.windowID == getId()) {
						quit();
					}
					break;
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					fmt::println("SDL_WINDOWEVENT_SIZE_CHANGED");
					resize(windowEvent.window.data1, windowEvent.window.data2);
					break;
				case SDL_WINDOWEVENT_RESIZED:
					fmt::println("SDL_WINDOWEVENT_RESIZED");
					break;
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					if (windowEvent.window.windowID == getId()) {
						focus_ = true;
					}
					break;
				case SDL_WINDOWEVENT_FOCUS_LOST:
					if (windowEvent.window.windowID == getId()) {
						focus_ = false;
					}
					break;
			}
			break;
		case SDL_KEYDOWN:
			switch (windowEvent.key.keysym.sym) {
				case SDLK_c:
					for (auto& gamepad : gameControllers_) {
						fmt::println("{}", gamepad.getName());
					}
					break;
				case SDLK_SPACE:
					if (onSpacePressed_) {
						onSpacePressed_();
					}
					break;
				case SDLK_ESCAPE:
					if (focus_) {
						quit();
					}
					break;
			}
			break;
		case SDL_MOUSEMOTION:
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (windowEvent.button.button == SDL_BUTTON_LEFT) {
				if (windowEvent.button.clicks == 2) {
					setFullScreen(!isFullScreen());
				}
			}
			fmt::println("DOWN");
			break;
		case SDL_MOUSEBUTTONUP:
			fmt::println("UP");
			break;
		case SDL_CONTROLLERDEVICEADDED:
			fmt::println("\nSDL_CONTROLLERDEVICEADDED");
			fmt::println("ControllerEvent: {}", ++controllerEvent_);
			fmt::println("Timestamp: {}", windowEvent.cdevice.timestamp);
			fmt::println("Type: {}", windowEvent.cdevice.type);
			fmt::println("Which (Joystick index): {}", windowEvent.cdevice.which);
			gameControllers_.push_back(sdl::GameController::addController(windowEvent.cdevice.which));
			break;
		case SDL_CONTROLLERDEVICEREMAPPED:
			fmt::println("SDL_CONTROLLERDEVICEREMOVED");
			fmt::println("ControllerEvent: {}", ++controllerEvent_);
			fmt::println("Timestamp: {}", windowEvent.cdevice.timestamp);
			fmt::println("Type: {}", windowEvent.cdevice.type);
			fmt::println("Which (InstanceId): {}", windowEvent.cdevice.which);
			break;
		case SDL_CONTROLLERDEVICEREMOVED:
			fmt::println("\nSDL_CONTROLLERDEVICEREMOVED");
			fmt::println("ControllerEvent: {}", ++controllerEvent_);
			fmt::println("Timestamp: {}", windowEvent.cdevice.timestamp);
			fmt::println("Type: {}", windowEvent.cdevice.type);
			fmt::println("Which (InstanceId): {}", windowEvent.cdevice.which);
			removeGamepad(windowEvent.cdevice.which);
			break;
		case SDL_CONTROLLERBUTTONDOWN:
			printGameControllerButton(windowEvent.cbutton.button);
			fmt::println("SDL_CONTROLLERBUTTONDOWN");
			fmt::println("ControllerEvent: {}", ++controllerEvent_);
			fmt::println("Timestamp: {}", windowEvent.cbutton.timestamp);
			fmt::println("Type: {}", windowEvent.cbutton.type);
			fmt::println("Which: {}", windowEvent.cbutton.which);
			break;
		case SDL_CONTROLLERBUTTONUP:
			break;
		case SDL_CONTROLLERAXISMOTION:
			switch (windowEvent.caxis.axis) {
				case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
					if (windowEvent.caxis.value > 15000) {
						fmt::println("SDL_CONTROLLER_AXIS_TRIGGERLEFT");
					}
					break;
				case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
					if (windowEvent.caxis.value > 15000) {
						fmt::println("SDL_CONTROLLER_AXIS_TRIGGERRIGHT");
					}
					break;
			}

			break;
	}
}

void TestWindow::resize(int w, int h) {
	gl::glViewport(0, 0, w, h);
	//shader_->setProjectionMatrixU(mw::getOrthoProjectionMatrix44<GLfloat>(0, (GLfloat) w, 0, (GLfloat) h));
	//shader_->setProjectionMatrixU(mw::getOrthoProjectionMatrix44<GLfloat>(0, (GLfloat) w, 0, (GLfloat) h));
	//shader_->setProjectionMatrix(glm::ortho(-0.5f * sprite_.getWidth(), 0.5f * sprite_.getHeight(), -0.5f * sprite_.getHeight(), 0.5f * sprite_.getHeight()));
}

void TestWindow::removeGamepad(SDL_JoystickID instanceId) {
	auto it = std::find_if(gameControllers_.begin(), gameControllers_.end(), [instanceId](const sdl::GameController& gameController) {
		return gameController.getInstanceId() == instanceId;
	});

	if (it != gameControllers_.end()) {
		gameControllers_.erase(it);
		spdlog::info("Gamepad removed: {}", instanceId);
	} else {
		spdlog::warn("Gamepad failed to be removed: {}", instanceId);
	}
}

void TestWindow::initPreLoop() {
	setLoopSleepingTime(std::chrono::milliseconds{10});
	sdl::Font font{"Ubuntu-B.ttf", 60};
	shader_ = std::make_shared<TestShader>("testShader_330.ver.glsl", "testShader_330.fra.glsl");
	batch_ = std::make_shared<BatchTriangles>(shader_, gl::GL_DYNAMIC_DRAW);
	shader_->useProgram();
	gl::glClearColor(0, 0, 0, 1);
	resize(getWidth(), getHeight());
	
	batch_->init();
	batch_->addRectangle(-1.f, -1.f, 2.f, 2.f, sprite_);
	batch_->uploadToGraphicCard();
	
	batch2_ = std::make_shared<BatchTriangles>(shader_, gl::GL_DYNAMIC_DRAW);
	text_ = sdl::Sprite{"HOHO", font};
	batch2_->init();
	batch2_->addRectangle(0, 0, 1, 1, text_);
	batch2_->uploadToGraphicCard();

	sdl::GameController::loadGameControllerMappings("gamecontrollerdb.txt");
	setHitTestCallback([](const SDL_Point&) {
		return SDL_HITTEST_DRAGGABLE;
	});
}
