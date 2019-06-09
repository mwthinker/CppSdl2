#ifndef CPPSDL2_SDL_INITTTF_H
#define CPPSDL2_SDL_INITTTF_H

#include <SDL_ttf.h>

namespace sdl {

	class InitTtf {
	protected:
		InitTtf();
		~InitTtf();

		InitTtf(const InitTtf&);
		InitTtf& operator=(const InitTtf&);

	private:
		static int nbrOfInstances;
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_INITTTF_H
