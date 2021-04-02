#ifndef TESTWINDOW2_H
#define TESTWINDOW2_H

#include "types.h"

#include <sdl/window.h>
#include <sdl/textureatlas.h>
#include <sdl/window.h>
#include <sdl/shader.h>
#include <sdl/graphic.h>


class GraphicWindow : public sdl::Window {
public:
	GraphicWindow(int majorGlVersion, int minorGlVersion)
		: Window{majorGlVersion, minorGlVersion} {
	}

private:
	void initPreLoop() override {
		sprite_ = textureAtlas_.add("tetris.bmp");

		

		resize(sdl::Window::getWidth(), sdl::Window::getHeight());
		shader_ = sdl::Shader::CreateShaderGlsl_330();
		
		shader_.setMatrix(Mat44{1});
		
		graphic_.addRectangle({ 0.1f, 0.4f }, { 0.2f, 0.2f }, sdl::color::White);
		graphic_.addRectangle({ 0.1f, 0.4f }, { 0.2f, 0.2f }, sdl::color::White);
		//graphic_.addTriangle({0, 0}, {0.3f, 0}, {0.3f, 0.3f});
		graphic_.addCircle({ -0.5f, 0.5f }, 0.3f, sdl::color::White, 40);
		//graphic_.addRectangle({-0.3f, -0.2f}, {0, 0}, {0.f, 0.1f}, {-0.2f, 0.2f});

		//graphic_.addCircle(0.5f, -0.5f, 0.3f, 0.6f, 30);

		//graphic_.addLine(-0.7f, -0.9f, -0.7f, -0.1f, 0.2f);
		for (float angle = 0.0; angle < 2 * 3.14; angle += 2 * 3.14f / 16) {
			//batch_->addLine(0.7f * std::cos(angle), 0.7f * std::sin(angle), 0.2f * std::cos(angle), 0.2f * std::sin(angle), 0.05f);
		}
		
		//batch_->addRectangle(-0.9f, -0.9f, 0.3f, 0.3f, sprite_);
		
		sprite_.bindTexture();
		sdl::assertGlError();
	}

	void update(const sdl::DeltaTime& deltaTime) override {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		sprite_.bindTexture();
		graphic_.draw(shader_);

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
						//batch_->addLine(0.7f, -0.7f, 0.7f, 0.7f, 0.2f);
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

	sdl::Shader shader_;
	sdl::Graphic graphic_;
	sdl::Sprite sprite_;
	sdl::TextureAtlas textureAtlas_{2048, 2048};
};

#endif
