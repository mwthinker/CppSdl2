#ifndef CPPSDL2_SDL_OPENGL_H
#define CPPSDL2_SDL_OPENGL_H


#ifdef _WIN32
#include <windows.h>
#endif

#include <glad/glad.h>

namespace sdl {

// Function checkGlError() prints all OpenGL errors during debug mode.
#if _DEBUG

	// Print all OpenGL errors.
	// Call check_gl_error instead if the error checking only should be in debug mode.
	void _assertGlError(const char* file, int line);

#define assertGlError() _assertGlError(__FILE__,__LINE__)
#else

	inline void _empty() {}

#define assertGlError() _empty()
#endif

}

#endif
