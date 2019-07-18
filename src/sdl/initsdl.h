#ifndef CPPSDL2_SDL_INITSDL_H
#define CPPSDL2_SDL_INITSDL_H

#include <SDL.h>

namespace sdl {

	class InitSdl {
	public:
		InitSdl(Uint32 flags = SDL_INIT_VIDEO);
		~InitSdl();
		
		InitSdl(const InitSdl&) = delete;
		InitSdl& operator=(const InitSdl&) = delete;

		Uint32 getFlags() const {
			return flags_;
		}

	private:
		void initSdl();
		void initMixer();
		void initJoystick();

		Uint32 flags_;
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_INITSDL_H
