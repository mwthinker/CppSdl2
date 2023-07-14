#include "gamecontroller.h"

#include <spdlog/spdlog.h>
#include <SDL.h>

#include <cstring>

namespace sdl {

	std::string guidToString(const SDL_JoystickGUID& guid) {
		std::string str(33, '0'); // SDL_JoystickGetGUIDString requires size >= 33
		SDL_JoystickGetGUIDString(guid, str.data(), static_cast<int>(str.size()));
		return str;
	}

	bool operator==(const SDL_JoystickGUID& guid1, const SDL_JoystickGUID& guid2) {
		return std::memcmp(guid1.data, guid2.data, sizeof(SDL_JoystickGUID::data)) == 0;
	}
	bool operator!=(const SDL_JoystickGUID& guid1, const SDL_JoystickGUID& guid2) {
		return !(guid1 == guid2);
	}

	GameController::GameController(SDL_GameController* gameController, const SDL_JoystickGUID& guid) noexcept
		: gameController_{gameController}
		, guid_{guid} {
	}

	GameController::GameController(GameController&& other) noexcept
		: gameController_{std::exchange(other.gameController_, nullptr)}
		, guid_{std::exchange(other.guid_, {})} {
	}

	GameController& GameController::operator=(GameController&& other) noexcept {
		if (gameController_ != nullptr) {
			SDL_GameControllerClose(gameController_);
		}
		gameController_ = std::exchange(other.gameController_, nullptr);
		guid_ = std::exchange(other.guid_, {});
		return *this;
	}

	GameController::~GameController() {
		if (gameController_ != nullptr) {
			SDL_GameControllerClose(gameController_);
		}
	}

	bool operator==(const GameController& gameController, SDL_JoystickID joystickId) {
		return joystickId == (gameController.gameController_ != nullptr ? gameController.getInstanceId() : 0);
	}

	bool operator==(SDL_JoystickID joystickId, const GameController& gameController) {
		return gameController == joystickId;
	}

	bool operator!=(const GameController& gameController, SDL_JoystickID joystickId) {
		return !(gameController == joystickId);
	}

	bool operator!=(SDL_JoystickID joystickId, const GameController& gameController) {
		return !(gameController == joystickId);
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

				return GameController{controller, SDL_JoystickGetDeviceGUID(index)};
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

	const SDL_JoystickGUID& GameController::GameController::getGuid() const {
		return guid_;
	}

}
