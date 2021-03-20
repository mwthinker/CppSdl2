#include "opengl.h"

#include <spdlog/spdlog.h>

namespace sdl {

	namespace {

		constexpr const char* errorString(GLenum error) {
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
				case GL_CONTEXT_LOST:
					return "GL_CONTEXT_LOST";
			}
			return "Unknown GL error code";
		}
	}

	void _assertGlError(const char* file, int line) {
		while (GLenum error = glGetError()) {
			spdlog::warn("[sdl::OpenGl] OpenGl error: {}={} - {}: {}", error, errorString(error), file, line);
		}
	}

}
