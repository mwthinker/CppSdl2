#ifndef CPPSDL2_SDL_WINDOW_H
#define CPPSDL2_SDL_WINDOW_H

#include "opengl.h"

#include <SDL.h>

#include <chrono>
#include <string>
#include <utility>

namespace sdl {

	// Create a window which handle all user input. The graphic is rendered using OpenGL.
	// The OpenGL view port is the whole window size and the model view is the same size as
	// the window and origo is on the left down side.
	class Window {
	public:
		Window() = default;

		Window(int majorVersionGl, int minorVersionGl);

		virtual ~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		Window(Window&&) = delete;
		Window& operator=(Window&&) = delete;

		// Start the loop which handle all inputs and graphics in the windows. It will not
		// return until the loop is ended. Is closed when the windows is closed, i.e. a
		// call to the function quit().
		void startLoop();

		// Set the program to full screen mode or desktop mode.
		// If mode is not changed from the current mode nothing happen.
		void setFullScreen(bool fullScreen);

		// Return true if the program is in full screen mode.
		bool isFullScreen() const;

		std::pair<int, int> getSize() const;

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

		void setPosition(int x, int y);

		void setBordered(bool bordered);

		void setResizeable(bool resizable);

		void setIcon(const std::string& icon);

		void setTitle(const std::string& title);

		void setWindowSize(int width, int height);

		int getOpenGlMajorVersion() const noexcept;

		int getOpenGlMinorVersion() const noexcept;

		void setGlClear(GLbitfield glBitfield) noexcept;

		GLbitfield getGlClear() const noexcept;
		
		void setLoopSleepingTime(const std::chrono::nanoseconds& delay) noexcept;
		
		std::chrono::nanoseconds getLoopSleepingTime() const noexcept;

	protected:
		virtual void initOpenGl();

		virtual void initPreLoop() {
		}

	private:
		static constexpr int DEFAULT_MAJOR_VERSION_GL = 3;
		static constexpr int DEFAULT_MINOR_VERSION_GL = 3;
		static constexpr int DEFAULT_WIDTH = 800;
		static constexpr int DEFAULT_HEIGHT = 800;

		// Is called by the loop. The frequency in which this function is called is fixed
		// by the vertical frequency of the monitor (VSYNC) or the sleeping time of the loop.
		virtual void update(const std::chrono::high_resolution_clock::duration& deltaTime) {
		}

		// Is called by the loop. Is called when ever a SDL_EVENT occurs.
		virtual void eventUpdate(const SDL_Event& windowEvent) {
		}

		void runLoop();

		void setupOpenGlContext();

		std::string title_;

		SDL_Window* window_{};
		SDL_GLContext glContext_{};
		SDL_Surface* icon_{};
		
		int width_ = DEFAULT_WIDTH;
		int height_ = DEFAULT_HEIGHT;
		int x_ = SDL_WINDOWPOS_UNDEFINED;
		int y_ = SDL_WINDOWPOS_UNDEFINED;
		
		std::chrono::nanoseconds sleepingTime_{};
		const int majorVersionGl_ = DEFAULT_MAJOR_VERSION_GL, minorVersionGl_ = DEFAULT_MINOR_VERSION_GL;
		
		GLbitfield glBitfield_ = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
		
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

} // Namespace sdl.

#endif // CPPSDL2_SDL_WINDOW_H
