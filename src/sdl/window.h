#ifndef CPPSDL2_SDL_WINDOW_H
#define CPPSDL2_SDL_WINDOW_H

#include "opengl.h"
#include "color.h"

#include <SDL.h>

#include <chrono>
#include <string>
#include <utility>
#include <functional>

namespace sdl {

	using Clock = std::chrono::high_resolution_clock;
	using DeltaTime = std::chrono::high_resolution_clock::duration;

	// Create a window which handle all user input. The graphic is rendered using OpenGL.
	class Window {
	public:
		using HitTestCallback = std::function<SDL_HitTestResult(const SDL_Point&)>;

		Window();

		Window(int majorVersionGl, int minorVersionGl);

		virtual ~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		Window(Window&&) = delete;
		Window& operator=(Window&&) = delete;

		// Start the loop which handle all inputs and graphics in the windows. It will not
		// return until the loop is ended. Is closed when the window is closed, i.e. a
		// call to the function quit().
		void startLoop();

		void setFullScreen(bool fullScreen);

		bool isFullScreen() const;

		std::pair<int, int> getSize() const;

		std::pair<int, int> getMinSize() const;

		std::pair<int, int> getMaxSize() const;

		std::pair<int, int> getDrawableSize() const;

		std::pair<int, int> getWindowPosition() const;

		// Return the current windows width in pixels.
		int getWidth() const noexcept;

		// Return the current windows height in pixels.
		int getHeight() const noexcept;

		// Make the program to quit as soon as the current frame is finished.
		// I.e. the loop in startLoop() will be made to stop and startLoop() will return.
		void quit() noexcept;

		// The id for the windows. Is the same as calling SDL_GetWindowID.
		Uint32 getId() const;

		// Return the window pointer. Use with care.
		SDL_Window* getSdlWindow() const noexcept;

		void setAlwaysOnTop(bool always);
		bool isAlwaysOnTop() const;

		void setOpacity(float value);
		float getOpacity() const;

		void setPosition(int x, int y);

		void setBordered(bool bordered);

		void setResizeable(bool resizable);

		void setIcon(const std::string& icon);

		void setTitle(const std::string& title);

		void setSize(int width, int height);

		void setMinSize(int width, int height);

		void setMaxSize(int width, int height);

		int getOpenGlMajorVersion() const noexcept;

		int getOpenGlMinorVersion() const noexcept;

		void setGlClearColor(Color color) noexcept;

		void setGlClear(GLbitfield glBitfield) noexcept;

		GLbitfield getGlClear() const noexcept;
		
		void setLoopSleepingTime(const std::chrono::nanoseconds& delay) noexcept;
		
		std::chrono::nanoseconds getLoopSleepingTime() const noexcept;

		void setHitTestCallback(HitTestCallback onHitTest);

		bool isHitTestCallbackSet() const {
			return static_cast<bool>(onHitTest_);
		}

	protected:
		virtual void initOpenGl() {}

		virtual void initPreLoop() {}

		// Is called each loop cycle.
		virtual void update(const DeltaTime& deltaTime) {}
		
		// Is called each loop cycle until all windowEvents are called.
		virtual void eventUpdate(const SDL_Event& windowEvent) {}

		SDL_GLContext getGlContext();

	private:
		static SDL_HitTestResult hitTestCallback(SDL_Window* sdlWindow, const SDL_Point* area, void* data);

		static constexpr int DefaultWidth{800};
		static constexpr int DefaultHeight{800};
		static constexpr int DefaultMajorVersionGl{3};
		static constexpr int DefaultMinorVersionGl{3};

		void runLoop();

		void setupOpenGlContext();

		std::string title_;

		HitTestCallback onHitTest_;
		SDL_Window* window_{};
		SDL_GLContext glContext_{};
		SDL_Surface* icon_{};
		
		int width_{DefaultWidth};
		int height_{DefaultHeight};
		int x_{SDL_WINDOWPOS_UNDEFINED};
		int y_{SDL_WINDOWPOS_UNDEFINED};
		int minWidth_{-1};
		int minHeight_{-1};
		int maxWidth_{-1};
		int maxHeight_{-1};
		
		std::chrono::nanoseconds sleepingTime_{};
		int majorVersionGl_{DefaultMajorVersionGl};
		int minorVersionGl_{DefaultMinorVersionGl};
		
		GLbitfield glBitfield_{GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT};
		Color clearColor_;
		
		bool alwaysOnTop_{false};
		float opacity_{1.f};
		bool quit_{};
		bool fullScreen_{};
		bool bordered_{true};
		bool resizable_{true};
	};

	inline void Window::quit() noexcept {
		quit_ = true;
	}

	inline Uint32 Window::getId() const {
		return SDL_GetWindowID(window_);
	}

	inline SDL_Window* Window::getSdlWindow() const noexcept {
		return window_;
	}

	inline int Window::getOpenGlMajorVersion() const noexcept {
		return majorVersionGl_;
	}

	inline int Window::getOpenGlMinorVersion() const noexcept {
		return minorVersionGl_;
	}

	inline void Window::setGlClearColor(Color color) noexcept {
		clearColor_ = color;
	}

	inline void Window::setGlClear(GLbitfield glBitfield) noexcept {
		glBitfield_ = glBitfield;
	}

	inline GLbitfield Window::getGlClear() const noexcept {
		return glBitfield_;
	}

	inline void Window::setLoopSleepingTime(const std::chrono::nanoseconds& delay) noexcept {
		sleepingTime_ = delay;
	}

	inline std::chrono::nanoseconds Window::getLoopSleepingTime() const noexcept {
		return sleepingTime_;
	}

}

#endif
