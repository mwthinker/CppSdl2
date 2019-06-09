#ifndef CPPSDL2_SDL_INITSDL_H
#define CPPSDL2_SDL_INITSDL_H

#include <SDL.h>

namespace sdl {

	class InitSdl {
	protected:
		InitSdl();
		~InitSdl();
		
		InitSdl(const InitSdl&) = delete;
		InitSdl& operator=(const InitSdl&) = delete;

	private:
		static int nbrOfInstances;
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_INITSDL_H
