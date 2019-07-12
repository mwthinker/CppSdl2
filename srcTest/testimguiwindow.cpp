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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    batch_->draw();

    glDisable(GL_BLEND);
}

void TestImGuiWindow::initOpenGl() {
	ImGuiWindow::initOpenGl();
}

void TestImGuiWindow::initPreLoop() {
	ImGuiWindow::initPreLoop();
    shader_ = std::make_shared<TestShader2>("testShader2_2_1.ver.glsl", "testShader2_2_1.fra.glsl");
    batch_ = std::make_shared<BatchTriangles>(shader_);

    shader_->setModelMatrix(Mat44(1));
    shader_->setProjectionMatrix(Mat44(1));


    batch_->addRectangle(0.1f, 0.4f, 0.2f, 0.2f);
    batch_->addTriangle(TestShader2::Vertex(0, 0), TestShader2::Vertex(0.3f, 0), TestShader2::Vertex(0.3f, 0.3f));
    batch_->addCircle(-0.5f, 0.5f, 0.3f, 40);
    batch_->addRectangle(TestShader2::Vertex(-0.3f, -0.2f), TestShader2::Vertex(0, 0), TestShader2::Vertex(0.f, 0.1f), TestShader2::Vertex(-0.2f, 0.2f));
    batch_->addHexagon(-0.1f, 0.1f, 0.3f);
    batch_->uploadToGraphicCard();
    batch_->init();
    sdl::checkGlError();
}
