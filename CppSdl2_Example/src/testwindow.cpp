#include "testwindow.h"

#include <sdl/sprite.h>
#include <sdl/window.h>
#include <sdl/opengl.h>
#include <sdl/sprite.h>
#include <sdl/gamecontroller.h>
#include <sdl/framebuffer.h>

#include <iostream>

namespace {

	void printGameControllerButton(Uint8 button) {
		switch (button) {
			case SDL_CONTROLLER_BUTTON_A:
				std::cout << "SDL_CONTROLLER_BUTTON_A";
				break;
			case SDL_CONTROLLER_BUTTON_B:
				std::cout << "SDL_CONTROLLER_BUTTON_B";
				break;
			case SDL_CONTROLLER_BUTTON_X:
				std::cout << "SDL_CONTROLLER_BUTTON_X";
				break;
			case SDL_CONTROLLER_BUTTON_Y:
				std::cout << "SDL_CONTROLLER_BUTTON_Y";
				break;
			case SDL_CONTROLLER_BUTTON_BACK:
				std::cout << "SDL_CONTROLLER_BUTTON_BACK";
				break;
			case SDL_CONTROLLER_BUTTON_GUIDE:
				std::cout << "SDL_CONTROLLER_BUTTON_GUIDE";
				break;
			case SDL_CONTROLLER_BUTTON_START:
				std::cout << "SDL_CONTROLLER_BUTTON_START";
				break;
			case SDL_CONTROLLER_BUTTON_LEFTSTICK:
				std::cout << "SDL_CONTROLLER_BUTTON_LEFTSTICK";
				break;
			case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
				std::cout << "SDL_CONTROLLER_BUTTON_RIGHTSTICK";
				break;
			case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
				std::cout << "SDL_CONTROLLER_BUTTON_LEFTSHOULDER";
				break;
			case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
				std::cout << "SDL_CONTROLLER_BUTTON_RIGHTSHOULDER";
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_UP:
				std::cout << "SDL_CONTROLLER_BUTTON_DPAD_UP";
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
				std::cout << "SDL_CONTROLLER_BUTTON_DPAD_DOWN";
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
				std::cout << "SDL_CONTROLLER_BUTTON_DPAD_LEFT";
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
				std::cout << "SDL_CONTROLLER_BUTTON_DPAD_RIGHT";
				break;
			case SDL_CONTROLLER_BUTTON_MAX:
				std::cout << "SDL_CONTROLLER_BUTTON_MAX";
				break;
		}
		std::cout << "\n";
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Update model matrix.
	shader_->useProgram();
	sprite_.bind();
	shader_->setModelMatrix(Mat44(1));
	shader_->setProjectionMatrix(Mat44(1));
	batch_->draw();

	text_.bind();
	batch2_->draw();

	glDisable(GL_BLEND);
	sdl::assertGlError();
}

void TestWindow::eventUpdate(const SDL_Event& windowEvent) {
	switch (windowEvent.type) {
		case SDL_QUIT:
			quit();
			break;
		case SDL_MOUSEWHEEL:
		{
			auto add = windowEvent.wheel.y * 5;
			auto [w, h] = getSize();
			std::cout << windowEvent.wheel.which << " add = " << add << "\n";
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
					std::cout << "SDL_WINDOWEVENT_SIZE_CHANGED" << std::endl;
					resize(windowEvent.window.data1, windowEvent.window.data2);
					break;
				case SDL_WINDOWEVENT_RESIZED:
					std::cout << "SDL_WINDOWEVENT_RESIZED" << std::endl;
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
						std::cout << gamepad.getName() << "\n";
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
			std::cout << "DOWN" << std::endl;
			break;
		case SDL_MOUSEBUTTONUP:
			std::cout << "UP" << std::endl;
			break;
		case SDL_CONTROLLERDEVICEADDED:
			std::cout << "SDL_CONTROLLERDEVICEADDED" << std::endl;
			std::cout << "ControllerEvent:" << ++controllerEvent_ << std::endl;
			std::cout << "Timestamp: " << windowEvent.cdevice.timestamp << std::endl;
			std::cout << "Type: " << windowEvent.cdevice.type << std::endl;
			std::cout << "Which: " << windowEvent.cdevice.which << std::endl;
			gameControllers_.push_back(sdl::GameController::addController(windowEvent.cdevice.which));
			break;
		case SDL_CONTROLLERDEVICEREMOVED:
			std::cout << "SDL_CONTROLLERDEVICEREMOVED" << std::endl;
			std::cout << "ControllerEvent:" << ++controllerEvent_ << std::endl;
			std::cout << "Timestamp: " << windowEvent.cdevice.timestamp << std::endl;
			std::cout << "Type: " << windowEvent.cdevice.type << std::endl;
			std::cout << "Which: " << windowEvent.cdevice.which << std::endl;
			removeGamepad(windowEvent.cdevice.which);
			break;
		case SDL_CONTROLLERBUTTONDOWN:
			printGameControllerButton(windowEvent.cbutton.button);
			std::cout << "SDL_CONTROLLERBUTTONDOWN" << std::endl;
			std::cout << "ControllerEvent:" << ++controllerEvent_ << std::endl;
			std::cout << "Timestamp: " << windowEvent.cbutton.timestamp << std::endl;
			std::cout << "Type: " << windowEvent.cbutton.type << std::endl;
			std::cout << "Which: " << windowEvent.cbutton.which << std::endl;
			break;
		case SDL_CONTROLLERBUTTONUP:
			break;
		case SDL_CONTROLLERAXISMOTION:
			switch (windowEvent.caxis.axis) {
				case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
					if (windowEvent.caxis.value > 15000) {
						std::cout << "SDL_CONTROLLER_AXIS_TRIGGERLEFT" << std::endl;
					}
					break;
				case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
					if (windowEvent.caxis.value > 15000) {
						std::cout << "SDL_CONTROLLER_AXIS_TRIGGERRIGHT" << std::endl;
					}
					break;
				default:
					break;
			}

			break;
	}
}

void TestWindow::resize(int w, int h) {
	glViewport(0, 0, w, h);
	//shader_->setProjectionMatrixU(mw::getOrthoProjectionMatrix44<GLfloat>(0, (GLfloat) w, 0, (GLfloat) h));
	//shader_->setProjectionMatrixU(mw::getOrthoProjectionMatrix44<GLfloat>(0, (GLfloat) w, 0, (GLfloat) h));
	//shader_->setProjectionMatrix(glm::ortho(-0.5f * sprite_.getWidth(), 0.5f * sprite_.getHeight(), -0.5f * sprite_.getHeight(), 0.5f * sprite_.getHeight()));
}

void TestWindow::removeGamepad(SDL_JoystickID instanceId) {
	auto it = std::find_if(gameControllers_.begin(), gameControllers_.end(), [instanceId](const sdl::GameController& gameController) {
		return gameController == instanceId;
	});
	if (it != gameControllers_.end()) {
		gameControllers_.erase(it);
		std::cout << "Gamepad removed: " << instanceId << "\n";
	} else {
		std::cout << "Gamepad faild to be removed: " << instanceId << "\n";
	}
}

void TestWindow::initPreLoop() {
	setLoopSleepingTime(std::chrono::milliseconds{10});
	sdl::Font font{"Ubuntu-B.ttf", 60};
	shader_ = std::make_shared<TestShader>("testShader_330.ver.glsl", "testShader_330.fra.glsl");
	batch_ = std::make_shared<BatchTriangles>(shader_, GL_DYNAMIC_DRAW);
	shader_->useProgram();
	glClearColor(0, 0, 0, 1);
	resize(getWidth(), getHeight());
	
	batch_->init();
	batch_->addRectangle(-1.f, -1.f, 2.f, 2.f, sprite_);
	batch_->uploadToGraphicCard();
	
	batch2_ = std::make_shared<BatchTriangles>(shader_, GL_DYNAMIC_DRAW);
	text_ = sdl::Sprite{"HOHO", font};
	batch2_->init();
	batch2_->addRectangle(0, 0, 1, 1, text_);
	batch2_->uploadToGraphicCard();

	sdl::GameController::loadGameControllerMappings("gamecontrollerdb.txt");
	setHitTestCallback([](const SDL_Point&) {
		return SDL_HITTEST_DRAGGABLE;
	});
}
