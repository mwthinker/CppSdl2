#include "testimguiwindow.h"

#include <sdl/imguiauxiliary.h>

#include <spdlog/spdlog.h>

namespace {

	void test() {
		sdl::BatchView<TestShader::Vertex> batchData;
		sdl::Batch<TestShader::Vertex> batch{GL_DYNAMIC_DRAW};
		
		batch.startBatchView();

		auto batchView = batch.getBatchView(GL_TRIANGLES);
		batch.draw(batchView);
	}

}

TestImGuiWindow::TestImGuiWindow() {
	sdl::Window::setSize(512, 512);
	sdl::Window::setTitle("Test");
	sdl::Window::setIcon("tetris.bmp");
	sdl::ImGuiWindow::setShowDemoWindow(true);
}

void TestImGuiWindow::imguiEventUpdate(const SDL_Event& windowEvent) {
	auto& io = ImGui::GetIO();
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

void TestImGuiWindow::imGuiUpdate(const sdl::DeltaTime& deltaTime) {
	ImGui::MainWindow("Main", [&]() {
		ImGui::Button("Hello", {100, 100});
		ImGui::Button("Hello2", {50, 50});
	});
}

void TestImGuiWindow::imGuiPreUpdate(const sdl::DeltaTime& deltaTime) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	batch_->draw();
	batchIndexes_->draw();
	glDisable(GL_BLEND);
}

void TestImGuiWindow::initPreLoop() {
	ImGuiWindow::initPreLoop();
    shader_ = std::make_shared<TestShader>("testShader2_330.ver.glsl", "testShader2_330.fra.glsl");
	shader_->setModelMatrix(Mat44{1});
	shader_->setProjectionMatrix(Mat44{1});
	
	initBatchTriangles();
	initBatchTrianglesIndexes();
	sdl::assertGlError();
}

void TestImGuiWindow::initBatchTriangles() {
	batch_ = std::make_shared<BatchTriangles>(shader_, GL_STATIC_DRAW);
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
	batchIndexes_ = std::make_shared<BatchTrianglesIndexes>(shader_, GL_STATIC_DRAW);
	batchIndexes_->addRectangle(-0.8f, -0.3f, 0.2f, 0.4f);
	batchIndexes_->addTriangle({0, 0}, {-0.3f, 0}, {-0.3f, -0.3f});
	batchIndexes_->addRectangle({0.8f, -0.7f}, {0.9f, -0.8f}, {0.8f, 0.8f}, {0.6f, 0.7f});
	batchIndexes_->addHexagon(-0.5f, 0.5f, 0.1f);
	batchIndexes_->addCircle(-0.7f, -0.7f, 0.1f, 20);
	batchIndexes_->init();
	batchIndexes_->uploadToGraphicCard();
	spdlog::info("Batch using glDrawElements, size: {} MiB", batchIndexes_->getVboSizeInMiB());
}
