#ifndef CPPSDL2_SDL_WINDOW_H
#define CPPSDL2_SDL_WINDOW_H

#include "opengl.h"

#include <string>
#include <functional>
#include <utility>

#include <SDL.h>

namespace sdl {

	// Create a window which handle all user input. The graphic is rendered using OpenGL.
	// The OpenGL view port is the whole window size and the model view is the same size as
	// the window and origo is on the left down side.
	class Window {
	public:
		Window();

		Window(int majorVersionGl, int minorVersionGl);

		virtual ~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

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

		// Return the current windows width in pixels.
		int getWidth() const;

		// Return the current windows height in pixels.
		int getHeight() const;

		// Make the program to quit as soon as the current frame is finished.
		// I.e. the loop in startLoop() will be made to stop and startLoop() will return.
		void quit();

		// The id for the windows. Is the same as calling SDL_GetWindowID.
		Uint32 getId() const;

		// Return the window pointer. Use with care.
		SDL_Window* getSdlWindow() const;

		void setPosition(int x, int y);

		void setBordered(bool bordered);

		void setResizeable(bool resizable);

		void setIcon(const std::string& icon);

		void setTitle(const std::string& title);

		void setWindowSize(int width, int height);

		int getOpenGlMajorVersion() const;

		int getOpenGlMinorVersion() const;

		void setGlClear(GLbitfield glBitfield);

		GLbitfield getGlClear() const;

		// Add a delay in the loop, add a sleeping time before frame swapping. Less than 10 ms may 
		// not effect anything, is plattform dependent.
		void setLoopSleepingTime(int sleepingTime);
		
		int getLoopSleepingTime() const;

	protected:
		virtual void initOpenGl();

		virtual void initPreLoop() {
		}

	private:
		// Is called by the loop. The frequency in which this function is called is fixed
		// by the vertical frequency of the monitor (VSYNC) or the sleeping time of the loop.
		virtual void update(double deltaTime) {
		}

		// Is called by the loop. Is called when ever a SDL_EVENT occurs.
		virtual void eventUpdate(const SDL_Event& windowEvent) {
		}

		void runLoop();

		void setupOpenGlContext();

		SDL_Window* window_;
		SDL_GLContext glContext_;
		int width_, height_;
		bool quit_;
		bool fullScreen_;
		bool bordered_;
		bool resizable_;
		
		GLbitfield glBitfield_;
		int x_, y_;
		std::string title_;
		SDL_Surface* icon_;
		int sleepingTime_;

		const int majorVersionGl_, minorVersionGl_;
	};

	inline void Window::quit() {
		quit_ = true;
	}

	inline Uint32 Window::getId() const {
		return SDL_GetWindowID(window_);
	}

	inline SDL_Window* Window::getSdlWindow() const {
		return window_;
	}

	inline int Window::getOpenGlMajorVersion() const {
		return majorVersionGl_;
	}

	inline int Window::getOpenGlMinorVersion() const {
		return minorVersionGl_;
	}

	inline void Window::setGlClear(GLbitfield glBitfield) {
		glBitfield_ = glBitfield;
	}

	inline GLbitfield Window::getGlClear() const {
		return glBitfield_;
	}

	inline void Window::setLoopSleepingTime(int sleepingTime) {
		sleepingTime_ = sleepingTime;
	}

	inline int Window::getLoopSleepingTime() const {
		return sleepingTime_;
	}

} // Namespace sdl.

#endif // CPPSDL2_SDL_WINDOW_H
