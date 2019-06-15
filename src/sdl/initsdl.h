#ifndef CPPSDL2_SDL_INITSDL_H
#define CPPSDL2_SDL_INITSDL_H

#include <SDL.h>

namespace sdl {

	class InitSdl {
	public:
		InitSdl(Uint32 flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);
		~InitSdl();
		
		InitSdl(const InitSdl&) = delete;
		InitSdl& operator=(const InitSdl&) = delete;

	private:
		void initSdl();
		void initMixer();
		void initTtf();
		void initJoystick();

		static int nbrOfInstances;
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_INITSDL_H
