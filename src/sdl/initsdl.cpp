#include "initsdl.h"

#include <spdlog/spdlog.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

namespace sdl {

	InitSdl::InitSdl(Uint32 flags)
		: flags_{flags} {

		if (flags & SDL_INIT_VIDEO) {
			initSdl();
		}
		if (flags & SDL_INIT_AUDIO) {
			initMixer();
		}
		if (flags & SDL_INIT_GAMECONTROLLER) {
			initJoystick();
		}
	}

	InitSdl::~InitSdl() {
		if (flags_ & SDL_INIT_GAMECONTROLLER) {
			SDL_JoystickEventState(SDL_DISABLE);
		}
		if (flags_ & SDL_INIT_AUDIO) {
			Mix_AllocateChannels(0);
			Mix_CloseAudio();
		}
		if (flags_ & SDL_INIT_VIDEO) {
			TTF_Quit();
			SDL_Quit();
		}
	}

	void InitSdl::initSdl() {
		if (SDL_Init(flags_)) {
			spdlog::error("[sdl::InitSdl] Unable to init SDL: {}", SDL_GetError());
		} else {
			if (!TTF_WasInit() && TTF_Init() == -1) {
				spdlog::error("[sdl::InitTtf] InitTtf failed");
			}
		}
	}

	void InitSdl::initMixer() {
		int success = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
		Mix_AllocateChannels(16);
		if (success == -1) {
			spdlog::error("[sdl::InitMixer] Sound failed to initiate!");
		}
	}

	void InitSdl::initJoystick() {
		SDL_JoystickEventState(SDL_ENABLE);
	}

}
