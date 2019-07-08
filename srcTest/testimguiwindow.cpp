#include "testimguiwindow.h"

TestImGuiWindow::TestImGuiWindow() {
	sdl::Window::setWindowSize(512, 512);
	sdl::Window::setTitle("Test");
	sdl::Window::setIcon("tetris.bmp");
}

void TestImGuiWindow::eventUpdate(const SDL_Event& windowEvent) {
	sdl::ImGuiWindow::eventUpdate(windowEvent);

	switch (windowEvent.type) {
		case SDL_WINDOWEVENT:
			switch (windowEvent.window.event) {
				case SDL_WINDOWEVENT_LEAVE:
					break;
				case SDL_WINDOWEVENT_CLOSE:
					sdl::Window::quit();
			}
			break;
		case SDL_QUIT:
			sdl::Window::quit();
			break;
	}
}

void TestImGuiWindow::update(double deltaTime) {
	ImGuiWindow::update(deltaTime);
}

void TestImGuiWindow::initOpenGl() {
	ImGuiWindow::initOpenGl();
}

void TestImGuiWindow::initPreLoop() {
	ImGuiWindow::initPreLoop();
}
