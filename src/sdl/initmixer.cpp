#include "initmixer.h"
#include "sound.h"
#include "logger.h"

#include <map>
#include <memory>

namespace sdl {

	InitMixer::InitMixer() {
		if (nbrOfInstances < 1) {
			int success = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
			Mix_AllocateChannels(16);
			if (success == -1) {
				logger()->error("[InitMixer] Sound failed to initiate!");
			}
		}
		++nbrOfInstances;
	}

	InitMixer::~InitMixer() {
		--nbrOfInstances;
		if (nbrOfInstances < 1) {
			Mix_AllocateChannels(0);
			Mix_CloseAudio();
		}
	}

	int InitMixer::nbrOfInstances = 0;

} // Namespace sdl.
