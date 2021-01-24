#ifndef TESTWINDOW2_H
#define TESTWINDOW2_H

#include "testshader.h"
#include "batchtriangles.h"
#include "types.h"

#include <sdl/window.h>
#include <sdl/textureatlas.h>

class TestWindow2 : public sdl::Window {
public:
	TestWindow2(int majorGlVersion, int minorGlVersion)
		: Window{majorGlVersion, minorGlVersion} {
	}

private:
	void initPreLoop() override {
		shader_ = std::make_shared<TestShader>("testShader2_2_1.ver.glsl", "testShader2_2_1.fra.glsl");
		sprite_ = textureAtlas_.add("tetris.bmp");

		batch_ = std::make_shared<BatchTriangles>(shader_, GL_DYNAMIC_DRAW);

		resize(sdl::Window::getWidth(), sdl::Window::getHeight());
		shader_->setModelMatrix(Mat44{1});
		shader_->setProjectionMatrix(Mat44{1});
		
		batch_->addRectangle(0.1f, 0.4f, 0.2f, 0.2f);
		batch_->addTriangle({0, 0}, {0.3f, 0}, {0.3f, 0.3f});
		batch_->addCircle(-0.5f, 0.5f, 0.3f, 40);
		batch_->addRectangle({-0.3f, -0.2f}, {0, 0}, {0.f, 0.1f}, {-0.2f, 0.2f});

		batch_->addCircle(0.5f, -0.5f, 0.3f, 0.6f, 30);

		batch_->addLine(-0.7f, -0.9f, -0.7f, -0.1f, 0.2f);
		for (float angle = 0.0; angle < 2 * 3.14; angle += 2 * 3.14f / 16) {
			batch_->addLine(0.7f * std::cos(angle), 0.7f * std::sin(angle), 0.2f * std::cos(angle), 0.2f * std::sin(angle), 0.05f);
		}
		
		batch_->addRectangle(-0.9f, -0.9f, 0.3f, 0.3f, sprite_);
				
		batch_->init();
		batch_->uploadToGraphicCard();
		sprite_.bindTexture();
		sdl::assertGlError();
	}

	void update(const sdl::DeltaTime& deltaTime) override {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		sprite_.bindTexture();
		batch_->draw();

		glDisable(GL_BLEND);
	}

	void eventUpdate(const SDL_Event& windowEvent) override {
		switch (windowEvent.type) {
			case SDL_QUIT:
				quit();
				break;
			case SDL_WINDOWEVENT:
				switch (windowEvent.window.event) {
					case SDL_WINDOWEVENT_CLOSE:
						quit();
						break;
					case SDL_WINDOWEVENT_RESIZED:
						resize(windowEvent.window.data1, windowEvent.window.data2);
						break;
				}
				break;
			case SDL_KEYDOWN:
				switch (windowEvent.key.keysym.sym) {
					case SDLK_SPACE:
						batch_->addLine(0.7f, -0.7f, 0.7f, 0.7f, 0.2f);
						batch_->uploadToGraphicCard();
						break;
					case SDLK_ESCAPE:
						quit();
						break;
				}
				break;
		}

	}

	void resize(int w, int h) {
		sdl::assertGlError();
		glViewport(0, 0, w, h);
		//shader_.setProjectionMatrix(glm::ortho(0, w, 0, h));
		sdl::assertGlError();
	}

	std::shared_ptr<TestShader> shader_;
	std::shared_ptr<BatchTriangles> batch_;
	sdl::Sprite sprite_;
	sdl::TextureAtlas textureAtlas_{2048, 2048};
};

#endif
