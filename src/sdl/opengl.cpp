#include "opengl.h"

#include <spdlog/spdlog.h>

namespace sdl {

	namespace {

#if _DEBUG
		const char* errorString(gl::GLenum error) {
			switch (error) {
				case gl::GL_INVALID_ENUM:
					return "GL_INVALID_ENUM";
				case gl::GL_INVALID_VALUE:
					return "GL_INVALID_VALUE";
				case gl::GL_INVALID_OPERATION:
					return "GL_INVALID_OPERATION";
				case gl::GL_STACK_UNDERFLOW:
					return "GL_STACK_UNDERFLOW";
				case gl::GL_OUT_OF_MEMORY:
					return "GL_OUT_OF_MEMORY";
				case gl::GL_INVALID_FRAMEBUFFER_OPERATION:
					return "GL_INVALID_FRAMEBUFFER_OPERATION";
				case gl::GL_CONTEXT_LOST:
					return "GL_CONTEXT_LOST";
			}
			return "Unknown GL error code";
		}
#endif

	}

#if __cpp_lib_source_location
	void assertGlError(std::source_location location) {
#if _DEBUG
		auto fileName = location.file_name();
		auto line = location.line();
		auto functionName = location.function_name();

		
		/* MW
		while (gl::GLenum error = gl::glGetError())
		{
			spdlog::warn("[sdl::OpenGl] File: {}, Line: {}, Function: {}, Error: {}={}", 
				fileName, line, functionName, error, errorString(error));
		}
		*/
#endif
	}
#endif

}
