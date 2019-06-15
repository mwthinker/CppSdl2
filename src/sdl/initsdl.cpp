#include "initsdl.h"
#include "logger.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

namespace sdl {

	InitSdl::InitSdl(Uint32 flags) {
		if (nbrOfInstances < 1) {
			initSdl();
			initMixer();
			initTtf();
			initJoystick();
		}
		++nbrOfInstances;
	}

	InitSdl::~InitSdl() {
		--nbrOfInstances;
		if (nbrOfInstances < 1) {
			SDL_JoystickEventState(SDL_DISABLE);
			TTF_Quit();
			Mix_AllocateChannels(0);
			Mix_CloseAudio();
			SDL_Quit();
		}
	}

	void InitSdl::initSdl() {
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER)) {
			logger()->error("[InitSdl] Unable to init SDL: {}", SDL_GetError());
		}
	}

	void InitSdl::initTtf() {
		if (!TTF_WasInit() && TTF_Init() == -1) {
			logger()->error("[InitTtf] InitTtf failed");
		}
	}

	void InitSdl::initMixer() {
		int success = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
		Mix_AllocateChannels(16);
		if (success == -1) {
			logger()->error("[InitMixer] Sound failed to initiate!");
		}
	}

	void InitSdl::initJoystick() {
		SDL_JoystickEventState(SDL_ENABLE);
	}

	int InitSdl::nbrOfInstances = 0;

} // Namespace sdl.
