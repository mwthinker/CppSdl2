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
	void addAxis() {
		// Axis x,y and z.
		graphic_.addRectangle({0.0f, 0.0f}, {0.1f, 0.01f}, sdl::color::Red);
		graphic_.addRectangle({0.0f, 0.0f}, {0.01f, 0.1f}, sdl::color::Green);
		graphic_.addCircle({0.0f, 0.0f}, 0.01f, sdl::color::Blue, 11);
	}

	void addSquare(sdl::Color color) {
		graphic_.addRectangle({-0.01f, -0.01f}, {0.02f, 0.02f}, color);
	}

	void initPreLoop() override {
		sprite_ = textureAtlas_.add("tetris.bmp");
		resize(sdl::Window::getWidth(), sdl::Window::getHeight());
		shader_ = sdl::Shader::CreateShaderGlsl_330();

		graphic_.addRectangle({0.1f, 0.4f}, {0.2f, 0.2f}, sdl::color::White);
		graphic_.addRectangle({-0.1f, -0.4f}, {0.2f, 0.2f}, sdl::color::Red);
		graphic_.addCircle({-0.5f, 0.5f}, 0.3f, sdl::color::Green, 40);

		addAxis();

		graphic_.pushMatrix([&] {
			graphic_.translate({-0.5f, -0.5f});
			addSquare(sdl::color::Red);
			
			graphic_.pushMatrix();
			graphic_.popMatrix();
			graphic_.translate({0.0f, 0.5f});
			addSquare(sdl::color::Green);
		});

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
					case SDLK_UP:
						graphic_.translate({0.0f, 0.2f});
						addSquare(sdl::color::html::Tomato);
						break;
					case SDLK_DOWN:
						graphic_.translate({0.0f, -0.2f});
						addSquare(sdl::color::html::Tomato);
						break;
					case SDLK_LEFT:
						graphic_.rotate(0.1f);
						addSquare(sdl::color::html::Tomato);
						break;
					case SDLK_RIGHT:
						graphic_.rotate(-0.1f);
						addSquare(sdl::color::html::Tomato);
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
