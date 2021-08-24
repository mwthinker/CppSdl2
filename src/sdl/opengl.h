#ifndef CPPSDL2_SDL_OPENGL_H
#define CPPSDL2_SDL_OPENGL_H

#include <glad/glad.h>
#include <tuple>

namespace sdl {
	
	template <typename... Caps>
	class GlEnableScoped {
	public:
		// Enable server-side GL capabilities.
		explicit GlEnableScoped(Caps... caps)
			: caps_{caps...}
		{
			static_assert(std::conjunction_v<std::is_same<int, Caps>...> || std::conjunction_v<std::is_same<GLenum, Caps>...>, "Must use int or GLenum");
			std::apply([](auto&&... caps) {
				((glEnable(caps)), ...);
				((assert(isValid(caps))), ...);
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
