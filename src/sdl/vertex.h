#ifndef CPPSDL2_SDL_VERTEX_H
#define CPPSDL2_SDL_VERTEX_H

#include "color.h"

#include <glm/glm.hpp>

namespace sdl {
	
	struct Vertex {
		glm::vec2 pos;
		glm::vec2 tex;
		Color color;
	};

}

#endif
