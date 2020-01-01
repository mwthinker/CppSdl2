#ifndef CPPSDL2_SDL_VERTEX_H
#define CPPSDL2_SDL_VERTEX_H

#include "color.h"

namespace sdl {
	
	struct Vertex {
		glm::vec2 pos{0, 0};
		glm::vec2 tex{0, 0};
		Color color{1, 1, 1, 1};
	};

} // Namespace vin.

#endif // CPPSDL2_SDL_VERTEX_H
