#include "window.h"
#include "exception.h"
#include "sprite.h"

#include <spdlog/spdlog.h>

#include <SDL_image.h>

#include <thread>
#include <chrono>
#include <sstream>
#include <stdexcept>

namespace sdl {

	namespace {
		
		void initGlad() {
			if (gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
				spdlog::info("[sdl::Window] gladLoadGLLoader succeeded");
			} else {
				spdlog::error("[sdl::Window] gladLoadGLLoader failed, something is seriously wrong");
				throw std::exception{};
			}
		}

		constexpr int DEFAULT_MAJOR_VERSION_GL{3};
		constexpr int DEFAULT_MINOR_VERSION_GL{3};

	}

	Window::Window()
		: Window{DEFAULT_MAJOR_VERSION_GL, DEFAULT_MINOR_VERSION_GL} {
	}

	Window::Window(int majorVersionGl, int minorVersionGl)
		: majorVersionGl_{majorVersionGl}
		, minorVersionGl_{minorVersionGl} {
		
		assert(majorVersionGl > 0 && minorVersionGl > 0);
		spdlog::info("[sdl::Window] Creating Window");
	}

	void Window::setupOpenGlContext() {
		glContext_ = SDL_GL_CreateContext(window_);
		if (glContext_ == 0) {
			spdlog::error("[sdl::Window] SDL_GL_CreateContext failed: ", SDL_GetError());
			throw std::exception{};
		}

		if (SDL_GL_SetSwapInterval(1) < 0) {
			spdlog::warn("[sdl::Window] Warning: Unable to set VSync! SDL Error: ", SDL_GetError());
		}
		
		initGlad();

        spdlog::info("[sdl::Window] Setup OpenGl version: {}.{}", majorVersionGl_, minorVersionGl_);
		if (char* version = (char*) glGetString(GL_VERSION)) {
			spdlog::info("[sdl::Window] GL_VERSION: {}", version);
			spdlog::info("[sdl::Window] GL_SHADING_LANGUAGE_VERSION: {}", (char*) glGetString(GL_SHADING_LANGUAGE_VERSION));
		} else {
			spdlog::error("[sdl::Window] Error: unknown OpenGL version loadad!");
			throw std::exception{};
		}
	}

	Window::~Window() {
		if (icon_) {
			SDL_FreeSurface(icon_);
		}

		if (window_ != nullptr) {
			// Clean up current OpenGL context and the window.
			SDL_GL_DeleteContext(glContext_);
			SDL_GL_UnloadLibrary();
			SDL_DestroyWindow(window_);
		}
	}

	void Window::startLoop() {
		if (window_) {
			return;
		}

		spdlog::info("[sdl::Window] Init loop");
		Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
		if (resizable_) {
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if (!bordered_) {
			flags |= SDL_WINDOW_BORDERLESS;
		}
		
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, majorVersionGl_);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minorVersionGl_);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#if _DEBUG
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif // _DEBUG
		initOpenGl();

		if (SDL_GL_LoadLibrary(nullptr) != 0) {
			spdlog::error("[sdl::Window] SDL_GL_LoadLibrary failed {}", SDL_GetError());
			spdlog::error("[sdl::Window] Failed to load OpenGL version {}.{}", majorVersionGl_, minorVersionGl_);
			throw std::exception{};
		}

		window_ = SDL_CreateWindow(
			title_.c_str(),
			x_,	y_,
			width_,	height_,
			flags
		);

		if (window_ == nullptr) {
			spdlog::error("[sdl::Window] SDL_CreateWindow failed: {}", SDL_GetError());
			throw std::exception();
		} else {
			spdlog::info("[sdl::Window] Windows {} created: \n\t(x, y) = ({}, {}) \n\t(w, h) = ({}, {}) \n\tflags = {}", title_, x_ == SDL_WINDOWPOS_UNDEFINED? -1 : x_, y_ == SDL_WINDOWPOS_UNDEFINED ? -1 : y_, width_, height_, flags);
		}

		if (icon_) {
			spdlog::debug("[sdl::Window] Windows icon updated");
			SDL_SetWindowIcon(window_, icon_);
			SDL_FreeSurface(icon_);
			icon_ = nullptr;
		}
		quit_ = false;

		setupOpenGlContext();
		initPreLoop();
		runLoop();
		spdlog::info("[sdl::Window] Loop ended");
	}

	void Window::runLoop() {
		spdlog::info("[sdl::Window] Loop starting");
		auto time = std::chrono::high_resolution_clock::now();
		while (!quit_) {
			glClear(glBitfield_);
				
			SDL_Event eventSDL;
			while (SDL_PollEvent(&eventSDL)) {
				eventUpdate(eventSDL);
			}

			auto currentTime = std::chrono::high_resolution_clock::now();
			auto delta = currentTime - time;
			time = currentTime;
			update(delta);

			if (sleepingTime_ > std::chrono::nanoseconds{0}) {
				std::this_thread::sleep_for(sleepingTime_);
			}
			SDL_GL_SwapWindow(window_);
		}
	}

	void Window::setBordered(bool bordered) {
		if (window_) {
			SDL_SetWindowBordered(window_, bordered ? SDL_TRUE : SDL_FALSE);
			spdlog::info("[sdl::Window] Window border: {}", bordered);
		} else {
			bordered_ = bordered;
		}
	}

	void Window::setPosition(int x, int y) {
		if (window_) {
			if (x_ < 0) {
				x_ = SDL_WINDOWPOS_UNDEFINED;
			}
			if (y_ < 0) {
				y_ = SDL_WINDOWPOS_UNDEFINED;
			}
			SDL_SetWindowPosition(window_, x, y);
			spdlog::info("[sdl::Window] Reposition window: (x, y) = ({}, {})", x_ == SDL_WINDOWPOS_UNDEFINED ? -1 : x_, y_ == SDL_WINDOWPOS_UNDEFINED ? -1 : y_);
		} else {
			x_ = x;
			y_ = y;
		}
	}

	void Window::setResizeable(bool resizable) {
		if (window_) {
			SDL_SetWindowResizable(window_, resizable ? SDL_TRUE : SDL_FALSE);
			spdlog::info("[sdl::Window] Window resizeable: {}", resizable);
		} else {
			resizable_ = resizable;
		}
	}

	void Window::setIcon(const std::string& icon) {
		if (icon_) {
			SDL_FreeSurface(icon_);
			icon_ = nullptr;
		}
		icon_ = IMG_Load(icon.c_str());
		if (window_) {
			if (icon_) {
				SDL_SetWindowIcon(window_, icon_);
				SDL_FreeSurface(icon_);
				icon_ = nullptr;
			}
		}
	}

	void Window::setTitle(const std::string& title) {
		if (window_) {
			spdlog::info("[sdl::Window] tile named to {}", title);
			SDL_SetWindowTitle(window_, title.c_str());
		} else {
			title_ = title;
		}
	}

	// Return true if the program is in full screen mode.
	bool Window::isFullScreen() const {
		if (window_) {
			return (SDL_GetWindowFlags(window_) & SDL_WINDOW_FULLSCREEN_DESKTOP) > 0;
		} else {
			return fullScreen_;
		}
	}

	std::pair<int, int> Window::getSize() const {
		if (window_) {
			std::pair<int, int> pair;
			SDL_GetWindowSize(window_, &pair.first, &pair.second);
			return pair;
		}
		return {width_, height_};;
	}

	std::pair<int, int> Window::getDrawableSize() const {
		if (window_) {
			std::pair<int, int> pair;
			SDL_GL_GetDrawableSize(window_, &pair.first, &pair.second);
			return pair;
		}
		return {width_, height_};;
	}

	std::pair<int, int> Window::getWindowPosition() const {
		if (window_) {
			std::pair<int, int> pair;
			SDL_GetWindowPosition(window_, &pair.first, &pair.second);
			return pair;
		}
		return {x_, y_};
	}

	void Window::setWindowSize(int width, int height) {
		if (window_) {
			spdlog::info("[sdl::Window] Resizing: (w, h) = ({}, {})", width, height);
			SDL_SetWindowSize(window_, width, height);
		} else {
			width_ = width;
			height_ = height;
		}
	}

	int Window::getWidth() const noexcept {
		return getSize().first;
	}

	int Window::getHeight() const noexcept {
		return getSize().second;
	}

	void Window::setFullScreen(bool fullScreen) {
		if (fullScreen == isFullScreen()) {
			return;
		}
		if (!window_) {
			fullScreen_ = fullScreen;
			return;
		}
		if (isFullScreen()) {
			spdlog::info("[sdl::Window] Fullscreen inactivated.");
			SDL_SetWindowFullscreen(window_, 0);
			SDL_SetWindowSize(window_, width_, height_);
			if (!bordered_) {
				spdlog::info("[sdl::Window] Border inactivated.");
				SDL_SetWindowBordered(window_, SDL_bool::SDL_FALSE);
			}
		} else {
			spdlog::info("[sdl::Window] Fullscreen activated.");
			SDL_GetWindowSize(window_, &width_, &height_);
			SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN_DESKTOP);
		}
	}

} // Namespace sdl.
