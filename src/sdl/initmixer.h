#ifndef CPPSDL2_SDL_INITMIXER_H
#define CPPSDL2_SDL_INITMIXER_H

#include <SDL_mixer.h>

namespace sdl {

	class InitMixer {
	protected:
		InitMixer();
		~InitMixer();

		InitMixer(const InitMixer&) = delete;
		InitMixer& operator=(const InitMixer&) = delete;

	private:
		static int nbrOfInstances;
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_INITMIXER_H
