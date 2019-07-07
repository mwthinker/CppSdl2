#include "testwindow.h"

#include <sdl/sprite.h>
#include <sdl/text.h>
#include <sdl/window.h>
#include <sdl/opengl.h>
#include <sdl/sprite.h>
#include <sdl/gamecontroller.h>

#include <iostream>

namespace {

	void printGameControllerButton(Uint8 button) {
		switch (button) {
			case SDL_CONTROLLER_BUTTON_A:
				std::cout << "SDL_CONTROLLER_BUTTON_A" << std::endl;
				break;
			case SDL_CONTROLLER_BUTTON_B:
				std::cout << "SDL_CONTROLLER_BUTTON_B" << std::endl;
				break;
			case SDL_CONTROLLER_BUTTON_X:
				std::cout << "SDL_CONTROLLER_BUTTON_X" << std::endl;
				break;
			case SDL_CONTROLLER_BUTTON_Y:
				std::cout << "SDL_CONTROLLER_BUTTON_Y" << std::endl;
				break;
			case SDL_CONTROLLER_BUTTON_BACK:
				std::cout << "SDL_CONTROLLER_BUTTON_BACK" << std::endl;
				break;
			case SDL_CONTROLLER_BUTTON_GUIDE:
				std::cout << "SDL_CONTROLLER_BUTTON_GUIDE" << std::endl;
				break;
			case SDL_CONTROLLER_BUTTON_START:
				std::cout << "SDL_CONTROLLER_BUTTON_START" << std::endl;
				break;
			case SDL_CONTROLLER_BUTTON_LEFTSTICK:
				std::cout << "SDL_CONTROLLER_BUTTON_LEFTSTICK" << std::endl;
				break;
			case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
				std::cout << "SDL_CONTROLLER_BUTTON_RIGHTSTICK" << std::endl;
				break;
			case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
				std::cout << "SDL_CONTROLLER_BUTTON_LEFTSHOULDER" << std::endl;
				break;
			case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
				std::cout << "SDL_CONTROLLER_BUTTON_RIGHTSHOULDER" << std::endl;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_UP:
				std::cout << "SDL_CONTROLLER_BUTTON_DPAD_UP" << std::endl;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
				std::cout << "SDL_CONTROLLER_BUTTON_DPAD_DOWN" << std::endl;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
				std::cout << "SDL_CONTROLLER_BUTTON_DPAD_LEFT" << std::endl;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
				std::cout << "SDL_CONTROLLER_BUTTON_DPAD_RIGHT" << std::endl;
				break;
			case SDL_CONTROLLER_BUTTON_MAX:
				std::cout << "SDL_CONTROLLER_BUTTON_MAX" << std::endl;
				break;
		}
	}

}

TestWindow::TestWindow(sdl::Sprite sprite) : Window(2, 1), sprite_(sprite) {
	sdl::Window::setWindowSize(512, 512);
	sdl::Window::setTitle("Test");
	sdl::Window::setIcon("tetris.bmp");
	controllerEvent_ = 0;
}

void TestWindow::update(double deltaTime) {
	Mat44 m = glm::ortho(-0.5f * sprite_.getWidth(), 0.5f * sprite_.getHeight(), -0.5f * sprite_.getHeight(), 0.5f * sprite_.getHeight());
	//Mat44 m = Mat44() sdl::getScaleMatrix44<float>(sprite_.getWidth(), sprite_.getHeight())*mw::getTranslateMatrix44<float>(0.5, 0.5);
	//Mat44 m2 = m * sdl::getTranslateMatrix44<float>(getWidth() * 0.5f, getHeight() * 0.5f) * mw::getScaleMatrix44<float>(sprite2_.getWidth(), sprite2_.getHeight());
	//glm::translate(m, )

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Update model matrix.
	shader_->useProgram();
	shader_->setColorU(1, 1, 1);
	shader_->setModelMatrixU(m);
	shader_->setTextureU(true);
	sprite_.bindTexture();
	//data1_->drawTRIANGLES();
	shader_->setColorU(1, 1, 1);
	//shader_->setModelMatrixU(m2);
	//data1_->drawTRIANGLES();
	shader_->setColorU(1, 0, 0);
	//shader_->setModelMatrixU(m2);
	
	//drawText_->draw();
	
	glDisable(GL_BLEND);
	sdl::checkGlError();
}

void TestWindow::eventUpdate(const SDL_Event& windowEvent) {
	switch (windowEvent.type) {
		case SDL_QUIT:
			quit();
			break;
		case SDL_MOUSEWHEEL:
			std::cout << windowEvent.wheel.which;
			break;
		case SDL_WINDOWEVENT:
			switch (windowEvent.window.event) {
				case SDL_WINDOWEVENT_CLOSE:
					if (windowEvent.window.windowID == getId()) {
						quit();
					}
					break;
				case SDL_WINDOWEVENT_RESIZED:
					resize(windowEvent.window.data1, windowEvent.window.data2);
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
					if (func_) {
						func_();
					}
					break;
				case SDLK_ESCAPE:
					if (focus_) {
						quit();
					}
					break;
			}
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
	shader_->setProjectionMatrixU(glm::ortho(-0.5f * sprite_.getWidth(), 0.5f * sprite_.getHeight(), -0.5f * sprite_.getHeight(), 0.5f * sprite_.getHeight()));
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
	setLoopSleepingTime(10);

	focus_ = true;
	shader_ = std::make_shared<TestShader>("testShader2_1.ver.glsl", "testShader2_1.fra.glsl");
	sdl::Font font("Ubuntu-B.ttf", 60);
	//text_ = sdl::Text("hej", font);
	shader_->useProgram();
	glClearColor(0, 0, 0, 1);
	resize(getWidth(), getHeight());
	//data1_ = std::make_shared<TestShaderData>(shader_);
	// Testing empty data.
	//data1_->begin();
	//data1_->addEmptySquareTRIANGLES();
	//data1_->end();
	// Use real data.
	//data1_->begin();
	//data1_->addSquareTRIANGLES(-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, sprite_);
	//data1_->end();

	//drawText_ = std::make_shared<DrawText>(shader_, text_, 0.f, 0.f);
	//buffer1_.addVertexData(data1_);
	//buffer1_.addVertexData(drawText_);
	//buffer1_.uploadToGraphicCard();

	sdl::GameController::loadGameControllerMappings("gamecontrollerdb.txt");
}
