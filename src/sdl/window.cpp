#include "window.h"
#include "exception.h"
#include "sprite.h"
#include "logger.h"

#include <SDL_image.h>

#include <chrono>
#include <sstream>
#include <stdexcept>

namespace sdl {

	namespace {
		
		void initGlew() {
			GLenum status = glewInit();
			if (GLEW_OK == status) {
				logger()->info("[Window] glewInit succeeded");
			} else {
				logger()->error("[Window] glewInit failed, something is seriously wrong: {}", glewGetErrorString(status));
				throw std::exception();
			}
			glGetError(); // Ignore, silly error which glew may cause.
		}

		constexpr int DEFAULT_MAJOR_VERSION_GL = 3;
		constexpr int DEFAULT_MINOR_VERSION_GL = 3;
	}

	Window::Window() : Window(DEFAULT_MAJOR_VERSION_GL, DEFAULT_MINOR_VERSION_GL) {
	}

	Window::Window(int majorVersionGl, int minorVersionGl) : window_(nullptr), x_(-1), y_(-1), icon_(nullptr), width_(800), height_(800),
		title_(""), resizable_(true), bordered_(true), fullScreen_(false), sleepingTime_(-1), quit_(false),
		glContext_(nullptr), glBitfield_(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT),
		majorVersionGl_(majorVersionGl), minorVersionGl_(minorVersionGl) {

		logger()->info("[Window] Creating Window");
	}

	void Window::setupOpenGlContext() {
		glContext_ = SDL_GL_CreateContext(window_);
		if (glContext_ == 0) {
			logger()->error("[Window] SDL_GL_CreateContext failed: ", SDL_GetError());
			throw std::exception();
		}

		if (SDL_GL_SetSwapInterval(1) < 0) {
			logger()->warn("[Window] Warning: Unable to set VSync! SDL Error: ", SDL_GetError());
		}

        logger()->info("[Window] Setup OpenGl version: {}.{}", majorVersionGl_, minorVersionGl_);
		if (char* version = (char*) glGetString(GL_VERSION)) {
			logger()->info("[Window] GL_VERSION: {}", version);
			logger()->info("[Window] GL_SHADING_LANGUAGE_VERSION: {}", (char*) glGetString(GL_SHADING_LANGUAGE_VERSION));
		} else {
			logger()->error("[Window] Error: unknown OpenGL version loadad!");
			throw std::exception();
		}

		initGlew();
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
		logger()->info("[Window] Init loop");
		if (!window_) {
			Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
			if (resizable_) {
				flags |= SDL_WINDOW_RESIZABLE;
			}

			if (!bordered_) {
				flags |= SDL_WINDOW_BORDERLESS;
			}

			if (x_ < 0) {
				x_ = SDL_WINDOWPOS_UNDEFINED;
			}
			if (y_ < 0) {
				y_ = SDL_WINDOWPOS_UNDEFINED;
			}

			initOpenGl();

			window_ = SDL_CreateWindow(
				title_.c_str(),
				x_,	y_,
				width_,	height_,
				flags
			);

			if (window_ == nullptr) {
				logger()->error("[Window] SDL_CreateWindow failed: {}", SDL_GetError());
				throw std::exception();
			} else {
				logger()->info("[Window] Windows {} created: \n\t(x, y) = ({}, {}) \n\t(w, h) = ({}, {}) \n\tflags = {}", title_, x_ == SDL_WINDOWPOS_UNDEFINED? -1 : x_, y_ == SDL_WINDOWPOS_UNDEFINED ? -1 : y_, width_, height_, flags);
			}

			if (icon_) {
				logger()->debug("[Window] Windows icon updated");
				SDL_SetWindowIcon(window_, icon_);
				SDL_FreeSurface(icon_);
				icon_ = nullptr;
			}
			quit_ = false;

			setupOpenGlContext();
			initPreLoop();
			runLoop();
		}
		logger()->info("[Window] Loop ended");
	}

	void Window::runLoop() {
		logger()->info("[Window] Loop starting");
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
			update(std::chrono::duration<double>(delta).count());

			if (sleepingTime_ >= 0) {
				SDL_Delay(sleepingTime_);
			}
			SDL_GL_SwapWindow(window_);
		}
	}

	void Window::setBordered(bool bordered) {
		if (window_) {
			SDL_SetWindowBordered(window_, bordered ? SDL_TRUE : SDL_FALSE);
			logger()->info("[Window] Window border: {}", bordered);
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
			logger()->info("[Window] Reposition window: (x, y) = ({}, {})", x_ == SDL_WINDOWPOS_UNDEFINED ? -1 : x_, y_ == SDL_WINDOWPOS_UNDEFINED ? -1 : y_);
		} else {
			x_ = x;
			y_ = y;
		}
	}

	void Window::setResizeable(bool resizable) {
		if (window_) {
			SDL_SetWindowResizable(window_, resizable ? SDL_TRUE : SDL_FALSE);
			logger()->info("[Window] Window resizeable: {}", resizable);
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
			logger()->info("[Window] tile named to {}", title);
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
		std::pair<int, int> pair;
		if (window_) {
			SDL_GetWindowSize(window_, &pair.first, &pair.second);
		} else {
			pair.first = width_;
			pair.second = height_;
		}
		return pair;
	}

	std::pair<int, int> Window::getDrawableSize() const {
		std::pair<int, int> pair;
		if (window_) {
			SDL_GL_GetDrawableSize(window_, &pair.first, &pair.second);
		} else {
			pair.first = width_;
			pair.second = height_;
		}
		return pair;
	}

	void Window::setWindowSize(int width, int height) {
		if (window_) {
			logger()->info("[Window] Resizing: (w, h) = ({}, {})", width, height);
			SDL_SetWindowSize(window_, width, height);
		} else {
			width_ = width;
			height_ = height;
		}
	}

	// Return the current windows width in pixels.
	int Window::getWidth() const {
		return getSize().first;
	}

	// Return the current windows height in pixels.
	int Window::getHeight() const {
		return getSize().second;
	}

	void Window::setFullScreen(bool fullScreen) {
		if (window_) {
			if (fullScreen != isFullScreen()) {
				if (isFullScreen()) {
					logger()->info("[Window] Fullscreen inactivated.");
					SDL_SetWindowFullscreen(window_, 0);
					SDL_SetWindowSize(window_, width_, height_);
					if (!bordered_) {
						logger()->info("[Window] Border inactivated.");
						SDL_SetWindowBordered(window_, SDL_bool::SDL_FALSE);
					}
				} else {
					logger()->info("[Window] Fullscreen activated.");
					SDL_GetWindowSize(window_, &width_, &height_);
					SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN_DESKTOP);
				}
			}
		} else {
			fullScreen_ = fullScreen;
		}
	}
	
	void Window::initOpenGl() {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, majorVersionGl_);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minorVersionGl_);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		if (SDL_GL_LoadLibrary(0) != 0) {
			logger()->error("[Window] SDL_GL_LoadLibrary failed {}", SDL_GetError());
			logger()->error("[Window] Failed to load OpenGL version {}.{}", majorVersionGl_, minorVersionGl_);
			throw std::exception();
		}
	}

} // Namespace sdl.
