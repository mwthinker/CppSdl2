#include "window.h"
#include "exception.h"
#include "sprite.h"

#include <spdlog/spdlog.h>
#include <glbinding/glbinding.h>
#include <glbinding/FunctionCall.h>
#include <glbinding/AbstractFunction.h>
#include <glbinding/gl/enum.h>
#include <SDL_image.h>

#include <thread>
#include <chrono>
#include <sstream>
#include <stdexcept>

namespace sdl {

	namespace {

		const char* errorString(gl::GLenum error) {
			switch (error) {
				case gl::GL_NO_ERROR:                          return "GL_NO_ERROR";
				case gl::GL_INVALID_ENUM:                      return "GL_INVALID_ENUM";
				case gl::GL_INVALID_VALUE:                     return "GL_INVALID_VALUE";
				case gl::GL_INVALID_OPERATION:                 return "GL_INVALID_OPERATION";
				case gl::GL_STACK_OVERFLOW:                    return "GL_STACK_OVERFLOW";
				case gl::GL_STACK_UNDERFLOW:                   return "GL_STACK_UNDERFLOW";
				case gl::GL_OUT_OF_MEMORY:                     return "GL_OUT_OF_MEMORY";
				case gl::GL_INVALID_FRAMEBUFFER_OPERATION:     return "GL_INVALID_FRAMEBUFFER_OPERATION";
				case gl::GL_TABLE_TOO_LARGE:                   return "GL_TABLE_TOO_LARGE";
			}
			return "Unknown GL error code";
		}

		void enableOpengGlDebug() {
			glbinding::setCallbackMaskExcept(glbinding::CallbackMask::After, {"glGetError"});
			glbinding::setAfterCallback([](const glbinding::FunctionCall& call) {
				const auto error = gl::glGetError();
				if (error != gl::GL_NO_ERROR) {
					spdlog::warn("{}={} {}", errorString(error), static_cast<int>(error), call.function->name());
				}
			});
		}
	}

	Window::Window() {
		spdlog::info("[sdl::Window] Creating Window");
	}

	Window::Window(int majorVersionGl, int minorVersionGl)
		: majorVersionGl_{majorVersionGl}
		, minorVersionGl_{minorVersionGl} {
		
		assert(majorVersionGl >= 3 && minorVersionGl >= 3);
		spdlog::info("[sdl::Window] Creating Window");
		
		SDL_version sdlVersion;
		SDL_GetVersion(&sdlVersion);
		spdlog::info("[sdl::Window] SDL Version: {}.{}.{}", sdlVersion.major, sdlVersion.minor, sdlVersion.patch);
	}

	void Window::setupOpenGlContext() {
		glContext_ = SDL_GL_CreateContext(window_);
		if (glContext_ == 0) {
			spdlog::error("[sdl::Window] SDL_GL_CreateContext failed: ", SDL_GetError());
			throw std::exception{};
		}

		if (SDL_GL_SetSwapInterval(1) < 0) {
			spdlog::warn("[sdl::Window] Unable to set VSync: ", SDL_GetError());
		}
		
		glbinding::initialize([](const char* name) {
			return reinterpret_cast<glbinding::ProcAddress>(SDL_GL_GetProcAddress(name));
		});

		spdlog::info("[sdl::Window] Setup OpenGl version: {}.{}", majorVersionGl_, minorVersionGl_);
		if (char* version = (char*) gl::glGetString(gl::GL_VERSION)) {
			spdlog::info("[sdl::Window] GL_VERSION: {}", version);
			spdlog::info("[sdl::Window] GL_SHADING_LANGUAGE_VERSION: {}", (char*) gl::glGetString(gl::GL_SHADING_LANGUAGE_VERSION));
		} else {
			spdlog::error("[sdl::Window] Unknown OpenGL version loadad!");
			throw std::exception{};
		}

		enableOpengGlDebug();
	}

	Window::~Window() {
		if (icon_) {
			SDL_FreeSurface(icon_);
		}

		if (window_ != nullptr) {
			SDL_GL_DeleteContext(glContext_);
			SDL_GL_UnloadLibrary();
			SDL_DestroyWindow(window_);
		}
		spdlog::debug("[sdl::Window] Destructed.");
	}

	void Window::startLoop() {
		if (window_) {
			spdlog::warn("[sdl::Window] Loop already running");
			return;
		}

		spdlog::info("[sdl::Window] Init loop");
		auto flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
		if (resizable_) {
			flags |= SDL_WINDOW_RESIZABLE;
		}
		if (alwaysOnTop_) {
			flags |= SDL_WINDOW_ALWAYS_ON_TOP;
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
#endif
		initOpenGl();

		if (SDL_GL_LoadLibrary(nullptr) != 0) {
			spdlog::error("[sdl::Window] SDL_GL_LoadLibrary failed: {}", SDL_GetError());
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
		
		if (minWidth_ > 0 && minHeight_ > 0) {
			SDL_SetWindowMinimumSize(window_, minWidth_, minHeight_);
		}
		if (maxWidth_ > 0 && maxHeight_ > 0) {
			SDL_SetWindowMaximumSize(window_, maxWidth_, maxHeight_);
		}
		setOpacity(opacity_);

		if (icon_) {
			spdlog::debug("[sdl::Window] Windows icon updated");
			SDL_SetWindowIcon(window_, icon_);
			SDL_FreeSurface(icon_);
			icon_ = nullptr;
		}
		quit_ = false;
		setHitTestCallback(onHitTest_);

		setupOpenGlContext();
		initPreLoop();
		runLoop();
		spdlog::info("[sdl::Window] Loop ended");
	}

	void Window::runLoop() {
		spdlog::info("[sdl::Window] Loop starting");
		auto time = Clock::now();
		while (!quit_) {
			gl::glClearColor(clearColor_.red(), clearColor_.green(), clearColor_.blue(), clearColor_.alpha());
			gl::glClear(glBitfield_);
			
			SDL_Event eventSDL;
			while (SDL_PollEvent(&eventSDL)) {
				eventUpdate(eventSDL);
			}

			auto currentTime = Clock::now();
			auto delta = currentTime - time;
			time = currentTime;
			update(delta);

			if (sleepingTime_ > std::chrono::nanoseconds{0}) {
				std::this_thread::sleep_for(sleepingTime_);
			}
			SDL_GL_SwapWindow(window_);
		}
	}

	void Window::setOpacity(float opacity) {
		if (window_) {
			SDL_SetWindowOpacity(window_, opacity);
		} else {
			opacity_ = opacity;
		}
	}

	float Window::getOpacity() const {
		float opacity = opacity_;
		if (window_) {
			SDL_GetWindowOpacity(window_, &opacity);
		}
		return opacity_;
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
			if (x < 0) {
				x = SDL_WINDOWPOS_UNDEFINED;
			}
			if (y < 0) {
				y = SDL_WINDOWPOS_UNDEFINED;
			}			
			SDL_SetWindowPosition(window_, x, y);
			spdlog::info("[sdl::Window] Reposition window: (x, y) = ({}, {})", x == SDL_WINDOWPOS_UNDEFINED ? -1 : x, y == SDL_WINDOWPOS_UNDEFINED ? -1 : y);
		} else {
			x_ = x;
			y_ = y;
		}
	}

	void Window::setAlwaysOnTop(bool always) {
		if (window_) {
			spdlog::warn("[sdl::Window] Not supported after window creation!");
		} else {
			alwaysOnTop_ = always;
		}
	}

	bool Window::isAlwaysOnTop() const {
		return alwaysOnTop_;
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
		if (window_ && icon_) {
			SDL_SetWindowIcon(window_, icon_);
			SDL_FreeSurface(icon_);
			icon_ = nullptr;
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

	bool Window::isFullScreen() const {
		if (window_) {
			return (SDL_GetWindowFlags(window_) & SDL_WINDOW_FULLSCREEN_DESKTOP) > 0;
		} else {
			return fullScreen_;
		}
	}

	Size Window::getSize() const {
		if (window_) {
			Size size;
			SDL_GetWindowSize(window_, &size.width, &size.height);
			return size;
		}
		return {width_, height_};;
	}

	Size Window::getMinSize() const {
		if (window_) {
			Size size;
			SDL_GetWindowMinimumSize(window_, &size.width, &size.height);
			return size;
		}
		return {minWidth_, minHeight_};;
	}

	Size Window::getMaxSize() const {
		if (window_) {
			Size size;
			SDL_GetWindowMaximumSize(window_, &size.width, &size.height);
			return size;
		}
		return {maxWidth_, maxHeight_};;
	}

	Size Window::getDrawableSize() const {
		if (window_) {
			Size size;
			SDL_GL_GetDrawableSize(window_, &size.width, &size.height);
			return size;
		}
		return {width_, height_};;
	}

	Position Window::getWindowPosition() const {
		if (window_) {
			Position position;
			SDL_GetWindowPosition(window_, &position.x, &position.y);
			return position;
		}
		return {x_, y_};
	}

	void Window::setSize(int width, int height) {
		if (window_) {
			if (width < 1 || height < 1) {
				spdlog::warn("[sdl::Window] Resizing: (w, h) = ({}, {})", width, height);
				return;
			}

			spdlog::info("[sdl::Window] Resizing: (w, h) = ({}, {})", width, height);
			SDL_SetWindowSize(window_, width, height);
		} else {
			width_ = width;
			height_ = height;
		}
	}

	void Window::setMinSize(int width, int height) {
		if (window_) {
			if (width < 1 || height < 1) {
				return;
			}

			spdlog::info("[sdl::Window] Setting min size: (w, h) = ({}, {})", width, height);
			SDL_SetWindowMinimumSize(window_, width, height);
		} else {
			minWidth_ = width;
			minHeight_ = height;
		}
	}

	void Window::setMaxSize(int width, int height) {
		if (window_) {
			assert(width > 0 && height > 0);

			spdlog::info("[sdl::Window] Setting max size: (w, h) = ({}, {})", width, height);
			SDL_SetWindowMaximumSize(window_, width, height);
		} else {
			maxWidth_ = width;
			maxHeight_ = height;
		}
	}

	int Window::getWidth() const noexcept {
		return getSize().width;
	}

	int Window::getHeight() const noexcept {
		return getSize().height;
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

	SDL_GLContext Window::getGlContext() {
		return glContext_;
	}

	SDL_HitTestResult Window::hitTestCallback(SDL_Window* sdlWindow, const SDL_Point* area, void* data) {
		return static_cast<Window*>(data)->onHitTest_(*area);
	}

	void Window::setHitTestCallback(HitTestCallback onHitTest) {
		onHitTest_ = std::move(onHitTest);
		if (onHitTest_) {
			if (SDL_SetWindowHitTest(window_, hitTestCallback, this) == -1) {
				spdlog::warn("[sdl::Window] Window hit test failed to be initiated: {}", SDL_GetError());
			}
		} else {
			SDL_SetWindowHitTest(window_, nullptr, this);
		}
	}

}
