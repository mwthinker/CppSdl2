#include "gamecontroller.h"
#include "logger.h"

#include <SDL.h>

namespace sdl {	

	GameController::GameController(SDL_GameController* gameController) noexcept : gameController_(gameController) {
	}

	GameController::GameController(GameController&& other) noexcept  : gameController_(other.gameController_) {
		other.gameController_ = nullptr;
	}

	GameController& GameController::operator=(GameController&& other) noexcept {
		gameController_ = other.gameController_;
		other.gameController_ = nullptr;
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
			logger()->error("[GameController] Failed to load game controller mappings from file, {}. Error: {}", file, SDL_GetError());
		}
	}

	GameController GameController::addController(int index) {
		if (SDL_IsGameController(index)) {
			SDL_GameController* controller = SDL_GameControllerOpen(index);
			if (controller) {
				return GameController(controller);
			} else {
				logger()->error("[GameController] Could not open gamecontroller: {}", SDL_GetError());
				return GameController(nullptr);
			}
		} else {
			logger()->error("[GameController] Game Controller mapping is not avvailable.");
		}
		return GameController(nullptr);
	}

	void GameController::removeController(GameController&& gameController) {
		SDL_GameControllerClose(gameController.getSdlGameController());
	}

} // Namespace sdl.
