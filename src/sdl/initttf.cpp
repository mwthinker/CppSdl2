#include "initttf.h"
#include "logger.h"

#include <SDL_ttf.h>

namespace sdl {

	InitTtf::InitTtf() {
		if (nbrOfInstances < 1) {
			if (!TTF_WasInit() && TTF_Init() == -1) {
				logger()->error("[InitTtf] InitTtf failed");
			}
		}
		++nbrOfInstances;
	}

	InitTtf::~InitTtf() {
		--nbrOfInstances;
		if (TTF_WasInit() && nbrOfInstances < 1) {
			TTF_Quit();
		}
	}

	InitTtf::InitTtf(const InitTtf&) {
		++nbrOfInstances;
	}

	InitTtf& InitTtf::operator=(const InitTtf&) {
		++nbrOfInstances;
		return *this;
	}

	int InitTtf::nbrOfInstances = 0;

} // Namespace sdl.
