#include "opengl.h"

#include <spdlog/spdlog.h>

namespace sdl {

	namespace {

		static const char* errorString(GLenum error) {
			switch (error) {
				case GL_INVALID_ENUM:
					return "GL_INVALID_ENUM";
				case GL_INVALID_VALUE:
					return "GL_INVALID_VALUE";
				case GL_INVALID_OPERATION:
					return "GL_INVALID_OPERATION";
				case GL_STACK_UNDERFLOW:
					return "GL_STACK_UNDERFLOW";
				case GL_OUT_OF_MEMORY:
					return "GL_OUT_OF_MEMORY";
				case GL_INVALID_FRAMEBUFFER_OPERATION:
					return "GL_INVALID_FRAMEBUFFER_OPERATION";
#ifdef GL_CONTEXT_LOST
				case GL_CONTEXT_LOST:
					return "GL_CONTEXT_LOST";
#endif 
			}
			return "Unknown GL error code";
		}
	}

#if __cpp_lib_source_location
	void assertGlError(std::source_location location) {
#if _DEBUG
		auto fileName = location.file_name();
		auto line = location.line();
		auto functionName = location.function_name();

		while (GLenum error = glGetError()) {
			spdlog::warn("[sdl::OpenGl] File: {}, Line: {}, Function: {}, Error: {}={}", 
				fileName, line, functionName, error, errorString(error));
		}
#endif
	}
#endif

}
