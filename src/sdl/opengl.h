#ifndef CPPSDL2_SDL_OPENGL_H
#define CPPSDL2_SDL_OPENGL_H

#include <globjects/globjects.h>
#include <glbinding/gl/gl.h>

#include <tuple>
#if __cpp_lib_source_location // TODO! Remove when clang and GCC has support.
#include <source_location>
#endif

namespace sdl {
		
	template <typename... Caps>
	requires std::conjunction_v<std::is_same<int, Caps>...> || std::conjunction_v<std::is_same<gl::GLenum, Caps>...>
	class GlEnableScoped {
	public:
		explicit GlEnableScoped(Caps... caps)
			: caps_{caps...} {
			std::apply([](auto&&... caps) {
				((glEnable(caps)), ...);
#if _DEBUG
				((assert(isValid(caps))), ...);
#endif
			}, caps_);
		}

		~GlEnableScoped() {
			std::apply([](auto&&... caps) {((gl::glDisable(caps)), ...); }, caps_);
		}

	private:
		// Check if value is valid (missing some values) https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glEnable.xhtml.
		static bool isValid(gl::GLenum value) {
			return
				value == gl::GLenum::GL_ALPHA_TEST ||
				value == gl::GLenum::GL_BLEND ||
				value == gl::GLenum::GL_CULL_FACE ||
				value == gl::GLenum::GL_DEBUG_OUTPUT ||
				value == gl::GLenum::GL_DEPTH_CLAMP ||
				value == gl::GLenum::GL_DEPTH_TEST ||
				value == gl::GLenum::GL_DITHER ||
				value == gl::GLenum::GL_FRAMEBUFFER_SRGB ||
				value == gl::GLenum::GL_LINE_SMOOTH ||
				value == gl::GLenum::GL_MULTISAMPLE ||
				value == gl::GLenum::GL_POLYGON_OFFSET_FILL ||
				value == gl::GLenum::GL_POLYGON_OFFSET_LINE ||
				value == gl::GLenum::GL_POLYGON_OFFSET_POINT ||
				value == gl::GLenum::GL_POLYGON_SMOOTH ||
				value == gl::GLenum::GL_SCISSOR_TEST ||
				value == gl::GLenum::GL_STENCIL_TEST ||
				value == gl::GLenum::GL_TEXTURE_CUBE_MAP_SEAMLESS ||
				value == gl::GLenum::GL_PROGRAM_POINT_SIZE;
		}

		std::tuple<Caps...> caps_;
	};

	// Print all OpenGL errors under debug

#if __cpp_lib_source_location

	void assertGlError(std::source_location location = std::source_location::current());
#else
	inline void assertGlError() {}
#endif
}

#endif
