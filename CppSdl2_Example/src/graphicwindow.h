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
		graphic_.addCircle({-0.5f, 0.5f}, 0.3f, sdl::color::Green);
		graphic_.addHexagon({0.5f, -0.5f}, 0.3f, 0.4f, sdl::color::Blue);
		graphic_.addCircleOutline({0.5f, 0.5f}, 0.3f, 0.01f, sdl::color::html::Aqua);
		graphic_.addLine({-0.3f, -0.3f}, {-0.4f, -0.7f}, 0.01f, sdl::color::html::Aqua);
		graphic_.addPolygon({{-0.1f, -0.1f}, {0.0f, 0.1f}, {0.1f, -0.1f}}, sdl::Color{1.f, 0.f, 0.f, 0.1f});

		addAxis();

		graphic_.pushMatrix([&] {
			graphic_.translate({-0.5f, -0.5f});
			addSquare(sdl::color::Red);
			
			graphic_.pushMatrix();
			graphic_.popMatrix();
			graphic_.translate({0.0f, 0.5f});
			addSquare(sdl::color::Green);
		});

		sprite_.bind();
		sdl::assertGlError();
	}

	void update(const sdl::DeltaTime& deltaTime) override {
		sdl::GlEnableScoped scoped{GL_BLEND, GL_LINE_SMOOTH};
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		sprite_.bind();
		graphic_.upload(shader_);
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
						//addSquare(sdl::color::html::Tomato);
						graphic_.addPixel({0.1f, 0.1f}, sdl::color::Red, 8.f);
						break;
					case SDLK_DOWN:
						graphic_.translate({0.0f, -0.2f});
						//addSquare(sdl::color::html::Tomato);
						graphic_.addPixelLine({{0.1f, 0.1f}, {-0.3f, 0.5f}, {0.2f, 0.3f}}, sdl::color::Red);
						break;
					case SDLK_LEFT:
						graphic_.rotate(0.1f);
						addSquare(sdl::color::html::Tomato);
						graphic_.addPixel({0.1f, 0.1f}, sdl::color::Blue);
						break;
					case SDLK_RIGHT:
						graphic_.rotate(-0.1f);
						addSquare(sdl::color::html::Tomato);
						graphic_.addPixel({0.1f, 0.1f}, sdl::color::html::DarkOrange, 2.f);
						break;
					case SDLK_ESCAPE:
						quit();
						break;
					case SDLK_c:
						graphic_.clear();
						break;
				}
				break;
		}

	}

	void resize(int w, int h) {
		glViewport(0, 0, w, h);
	}

	sdl::Shader shader_;
	sdl::Graphic graphic_;
	sdl::Sprite sprite_;
	sdl::TextureAtlas textureAtlas_{2048, 2048};
};

#endif
