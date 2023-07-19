#include "gamecontroller.h"

#include <spdlog/spdlog.h>
#include <SDL.h>

#include <cstring>

bool operator==(const SDL_JoystickGUID& guid1, const SDL_JoystickGUID& guid2) {
	return std::memcmp(guid1.data, guid2.data, sizeof(SDL_JoystickGUID::data)) == 0;
}
bool operator!=(const SDL_JoystickGUID& guid1, const SDL_JoystickGUID& guid2) {
	return !(guid1 == guid2);
}

namespace sdl {

	std::string guidToString(const SDL_JoystickGUID& guid) {
		std::string str(33, '0'); // SDL_JoystickGetGUIDString requires size >= 33
		SDL_JoystickGetGUIDString(guid, str.data(), static_cast<int>(str.size()));
		return str;
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

			if (auto controller = SDL_GameControllerOpen(index); controller != nullptr) {

				return GameController{controller, SDL_JoystickGetDeviceGUID(index)};
			} else {
				spdlog::error("[sdl::GameController] Could not open game controller: {}", SDL_GetError());
			}
		} else {
			spdlog::error("[sdl::GameController] Game Controller mapping is not available.");
		}
		return GameController{};
	}

	void GameController::removeController(GameController&& gameController) {
		gameController.close();
	}

	const SDL_JoystickGUID& GameController::GameController::getGuid() const {
		return guid_;
	}

	void GameController::close() {
		if (gameController_ != nullptr) {
			SDL_GameControllerClose(gameController_);
			gameController_ = nullptr;
			guid_ = {};
		}
	}

}
