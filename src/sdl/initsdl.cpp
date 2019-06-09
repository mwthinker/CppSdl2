#include "initsdl.h"
#include "logger.h"

#include <SDL.h>

namespace sdl {

	InitSdl::InitSdl() {
		if (nbrOfInstances < 1) {
			if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) ) {
				logger()->error("[InitSdl] Unable to init SDL: {}", SDL_GetError());
			}
		}
		++nbrOfInstances;
	}

	InitSdl::~InitSdl() {
		--nbrOfInstances;
		if (nbrOfInstances < 1) {
			SDL_Quit();
		}
	}

	int InitSdl::nbrOfInstances = 0;

} // Namespace sdl.
