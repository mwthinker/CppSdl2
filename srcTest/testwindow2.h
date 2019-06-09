#ifndef TESTWINDOW2_H
#define TESTWINDOW2_H

#include "testshader2.h"
#include "batchtriangles.h"
#include "definitions.h"

#include <sdl/window.h>
#include <sdl/textureatlas.h>

class TestWindow2 : public sdl::Window {
public:
	TestWindow2(int majorGlVersion, int minorGlVersion) : textureAtlas_(2048, 2048) {
		Window::setOpenGlVersion(majorGlVersion, minorGlVersion);
	}

private:
	void initPreLoop() override {
		shader_ = std::make_shared<TestShader2>("testShader2_2_1.ver.glsl", "testShader2_2_1.fra.glsl");
		sprite_ = textureAtlas_.add("tetris.bmp");

		batch_ = std::make_shared<BatchTriangles>(shader_);

		resize(sdl::Window::getWidth(), sdl::Window::getHeight());
		shader_->setModelMatrix(Mat44(1));
		shader_->setProjectionMatrix(Mat44(1));

		
		batch_->addRectangle(0.1f, 0.4f, 0.2f, 0.2f);
		batch_->addTriangle(TestShader2::Vertex(0, 0), TestShader2::Vertex(0.3f, 0), TestShader2::Vertex(0.3f, 0.3f));
		batch_->addCircle(-0.5f, 0.5f, 0.3f, 40);
		batch_->addRectangle(TestShader2::Vertex(-0.3f, -0.2f), TestShader2::Vertex(0, 0), TestShader2::Vertex(0.f, 0.1f), TestShader2::Vertex(-0.2f, 0.2f));

		batch_->addCircle(0.5f, -0.5f, 0.3f, 0.6f, 30);

		batch_->addLine(-0.7f, -0.9f, -0.7f, -0.1f, 0.2f);
		for (float angle = 0.0; angle < 2 * 3.14; angle += 2 * 3.14f / 16) {
			batch_->addLine(0.7f * std::cos(angle), 0.7f * std::sin(angle), 0.2f * std::cos(angle), 0.2f * std::sin(angle), 0.05f);
		}
		
		batch_->addRectangle(-0.9f, -0.9f, 0.3f, 0.3f, sprite_);
		
		batch_->uploadToGraphicCard();
		sprite_.bindTexture();
		sdl::checkGlError();
	}

	void update(double deltaTime) override {
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
		sdl::checkGlError();
		glViewport(0, 0, w, h);
		//shader_.setProjectionMatrix(glm::ortho(0, w, 0, h));
		sdl::checkGlError();
	}

	std::shared_ptr<TestShader2> shader_;
	std::shared_ptr<BatchTriangles> batch_;
	sdl::Sprite sprite_;
	sdl::TextureAtlas textureAtlas_;
};

#endif // TESTWINDOW2_H
