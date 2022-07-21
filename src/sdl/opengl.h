#ifndef CPPSDL2_SDL_OPENGL_H
#define CPPSDL2_SDL_OPENGL_H

#include <glad/glad.h>
#include <tuple>
#if __cpp_lib_source_location // TODO! Remove when clang and GCC has support.
#include <source_location>
#endif

namespace sdl {
		
	template <typename... Caps>
	requires std::conjunction_v<std::is_same<int, Caps>...> || std::conjunction_v<std::is_same<GLenum, Caps>...>
	class GlEnableScoped {
	public:
		explicit GlEnableScoped(Caps... caps)
			: caps_{caps...}
		{
			std::apply([](auto&&... caps) {
				((glEnable(caps)), ...);
#if _DEBUG
				((assert(isValid(caps))), ...);
#endif
			}, caps_);
		}

		~GlEnableScoped() {
			std::apply([](auto&&... caps) {((glDisable(caps)), ...); }, caps_);
		}

	private:
		// Check if value is valid (missing some values) https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glEnable.xhtml.
		static bool isValid(GLenum value) {
			return
				value == GL_ALPHA_TEST ||
				value == GL_BLEND ||
				value == GL_CULL_FACE ||
#ifdef GL_DEBUG_OUTPUT
				value == GL_DEBUG_OUTPUT ||
#endif
				value == GL_DEPTH_CLAMP ||
				value == GL_DEPTH_TEST ||
				value == GL_DITHER ||
				value == GL_FRAMEBUFFER_SRGB ||
				value == GL_LINE_SMOOTH ||
				value == GL_MULTISAMPLE ||
				value == GL_POLYGON_OFFSET_FILL ||
				value == GL_POLYGON_OFFSET_LINE ||
				value == GL_POLYGON_OFFSET_POINT ||
				value == GL_POLYGON_SMOOTH ||
				value == GL_SCISSOR_TEST ||
				value == GL_STENCIL_TEST ||
				value == GL_TEXTURE_CUBE_MAP_SEAMLESS ||
				value == GL_PROGRAM_POINT_SIZE;
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
