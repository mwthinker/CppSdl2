#ifndef CPPSDL2_SDL_RECT_H
#define CPPSDL2_SDL_RECT_H

#include <SDL_rect.h>

namespace sdl {

	using Rect = SDL_Rect;

	struct Position {
		int x, y;
	};

	struct Size {
		int width, height;
	};

}

#endif
