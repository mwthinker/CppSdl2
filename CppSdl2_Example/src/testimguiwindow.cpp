#include "testimguiwindow.h"

#include <sdl/imguiauxiliary.h>

#include <spdlog/spdlog.h>

namespace {

	[[maybe_unused]] void test() {
		sdl::BatchView<TestShader::Vertex> batchData;
		sdl::Batch<TestShader::Vertex> batch{gl::GL_DYNAMIC_DRAW};
		
		batch.startBatchView();

		auto batchView = batch.getBatchView(gl::GL_TRIANGLES);
		batch.draw(batchView);
	}

}

TestImGuiWindow::TestImGuiWindow() {
	sdl::Window::setSize(512, 512);
	sdl::Window::setTitle("Test");
	sdl::Window::setIcon("tetris.bmp");
	sdl::ImGuiWindow::setShowDemoWindow(true);
	sdl::ImGuiWindow::setShowColorWindow(true);
}

void TestImGuiWindow::imGuiEventUpdate(const SDL_Event& windowEvent) {
	switch (windowEvent.type) {
		case SDL_WINDOWEVENT:
			switch (windowEvent.window.event) {
				case SDL_WINDOWEVENT_LEAVE:
					break;
				case SDL_WINDOWEVENT_CLOSE:
					sdl::Window::quit();
					break;
			}
			break;
		case SDL_QUIT:
			sdl::Window::quit();
			break;
		case SDL_KEYDOWN:
			switch (windowEvent.key.keysym.sym) {
				case SDLK_ESCAPE:
					sdl::Window::quit();
					break;
			}
			break;
	}
}

void TestImGuiWindow::imGuiUpdate(const sdl::DeltaTime& deltaTime) {
	ImGui::MainWindow("Main", [&]() {
		ImGui::Button("Hello", {100, 100});
		ImGui::Button("Hello2", {50, 50});
	});
}

void TestImGuiWindow::imGuiPreUpdate(const sdl::DeltaTime& deltaTime) {
	gl::glEnable(gl::GL_BLEND);
	gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);
	batch_->draw();
	batchIndexes_->draw();
	gl::glDisable(gl::GL_BLEND);
}

void TestImGuiWindow::initPreLoop() {
	ImGuiWindow::initPreLoop();
    shader_ = std::make_shared<TestShader>("testShader_330.ver.glsl", "testShader_330.fra.glsl");
	shader_->setModelMatrix(Mat44{1});
	shader_->setProjectionMatrix(Mat44{1});
	
	initBatchTriangles();
	initBatchTrianglesIndexes();
}

void TestImGuiWindow::initBatchTriangles() {
	batch_ = std::make_shared<BatchTriangles>(shader_, gl::GL_STATIC_DRAW);
	batch_->addRectangle(0.1f, 0.4f, 0.2f, 0.2f);
	batch_->addTriangle({0, 0}, {0.3f, 0}, {0.3f, 0.3f});
	batch_->addCircle(-0.5f, 0.5f, 0.3f, 40);
	batch_->addRectangle({-0.3f, -0.2f}, {0, 0}, {0.f, 0.1f}, {-0.2f, 0.2f});
	batch_->addHexagon(-0.1f, 0.1f, 0.3f);
	
	batch_->init();
	batch_->uploadToGraphicCard();
	spdlog::info("Batch using glDrawArrays, size: {} MB", batch_->getVboSizeInMiB());
}

void TestImGuiWindow::initBatchTrianglesIndexes() {
	batchIndexes_ = std::make_shared<BatchTrianglesIndexes>(shader_, gl::GL_STATIC_DRAW);
	batchIndexes_->addRectangle(-0.8f, -0.3f, 0.2f, 0.4f);
	batchIndexes_->addTriangle({0, 0}, {-0.3f, 0}, {-0.3f, -0.3f});
	batchIndexes_->addRectangle({0.8f, -0.7f}, {0.9f, -0.8f}, {0.8f, 0.8f}, {0.6f, 0.7f});
	batchIndexes_->addHexagon(-0.5f, 0.5f, 0.1f);
	batchIndexes_->addCircle(-0.7f, -0.7f, 0.1f, 20);
	batchIndexes_->init();
	batchIndexes_->uploadToGraphicCard();
	spdlog::info("Batch using glDrawElements, size: {} MiB", batchIndexes_->getVboSizeInMiB());
}
