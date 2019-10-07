#ifndef CPPSDL2_SDL_INITSDL_H
#define CPPSDL2_SDL_INITSDL_H

#include <SDL.h>

namespace sdl {

	class InitSdl {
	public:
		explicit InitSdl(Uint32 flags = SDL_INIT_VIDEO);
		~InitSdl();
		
		InitSdl(const InitSdl&) = delete;
		InitSdl& operator=(const InitSdl&) = delete;

		Uint32 getFlags() const noexcept {
			return flags_;
		}

	private:
		void initSdl();
		void initMixer();
		void initJoystick();

		Uint32 flags_ = 0;
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_INITSDL_H
