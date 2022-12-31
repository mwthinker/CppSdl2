#ifndef CPPSDL2_SDL_OPENGL_H
#define CPPSDL2_SDL_OPENGL_H

#include <glbinding/gl/gl.h>

#include <tuple>

namespace sdl {
		
	template <typename... Caps>
	requires std::conjunction_v<std::is_same<gl::GLenum, Caps>...>
	class GlEnableScoped {
	public:
		explicit GlEnableScoped(Caps... caps)
			: caps_{caps...} {
			std::apply([](auto&&... caps) {
				((glEnable(caps)), ...);
			}, caps_);
		}

		~GlEnableScoped() {
			std::apply([](auto&&... caps) {
				((gl::glDisable(caps)), ...);
			}, caps_);
		}

	private:
		std::tuple<Caps...> caps_;
	};

}

#endif
