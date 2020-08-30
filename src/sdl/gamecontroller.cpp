#include "gamecontroller.h"

#include <spdlog/spdlog.h>
#include <SDL.h>

namespace sdl {	

	GameController::GameController(SDL_GameController* gameController) noexcept
		: gameController_{gameController} {
	}

	GameController::GameController(GameController&& other) noexcept
		: gameController_{std::exchange(other.gameController_, nullptr)} {
	}

	GameController& GameController::operator=(GameController&& other) noexcept {
		if (gameController_ != nullptr) {
			SDL_GameControllerClose(gameController_);
		}
		gameController_ = std::exchange(other.gameController_,  nullptr);
		return *this;
	}

	GameController::~GameController() {
		if (gameController_ != nullptr) {
			SDL_GameControllerClose(gameController_);
		}
	}

	bool GameController::operator==(SDL_JoystickID joystickId) const {
		return joystickId == (gameController_ != nullptr ? getInstanceId() : 0);
	}

	const char* GameController::getName() const {
		return SDL_GameControllerName(gameController_);
	}

	void GameController::loadGameControllerMappings(const std::string& file) {
		if (SDL_GameControllerAddMappingsFromFile(file.c_str()) < 0) {
			spdlog::error("[sdl::GameController] Failed to load game controller mappings from file, {}. Error: {}", file, SDL_GetError());
		}
	}

	GameController GameController::addController(int index) {
		if (SDL_IsGameController(index)) {
			
			if (auto controller = SDL_GameControllerOpen(index);
				controller != nullptr) {
				
				return GameController{controller};
			} else {
				spdlog::error("[sdl::GameController] Could not open gamecontroller: {}", SDL_GetError());
			}
		} else {
			spdlog::error("[sdl::GameController] Game Controller mapping is not avvailable.");
		}
		return GameController{};
	}

	void GameController::removeController(GameController&& gameController) {
		SDL_GameControllerClose(gameController.getSdlGameController());
	}

}
