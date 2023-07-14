#ifndef CPPSDL2_SDL_GAMECONTROLLER_H
#define CPPSDL2_SDL_GAMECONTROLLER_H

#include <string>

#include <SDL.h>

namespace sdl {

	std::string guidToString(const SDL_JoystickGUID& guid);

	bool operator==(const SDL_JoystickGUID& guid1, const SDL_JoystickGUID& guid2);
	bool operator!=(const SDL_JoystickGUID& guid1, const SDL_JoystickGUID& guid2);

	class GameController {
	public:
		static void loadGameControllerMappings(const std::string& file);

		static GameController addController(int index);

		static void removeController(GameController&& gameController);

		GameController() noexcept = default;
		~GameController();

		GameController(GameController&& other) noexcept;
		GameController& operator=(GameController&& other) noexcept;

		GameController(const GameController&) = delete;
		GameController& operator=(const GameController&) = delete;

		friend bool operator==(const GameController& gameController, SDL_JoystickID joystickId);
		friend bool operator==(SDL_JoystickID joystickId, const GameController& gameController);
		friend bool operator!=(const GameController& gameController, SDL_JoystickID joystickId);
		friend bool operator!=(SDL_JoystickID joystickId, const GameController& gameController);

		const char* getName() const;

		/// @brief Get the underlaying pointer (use with care!). Can be null.
		/// @return SDL_GameController pointer
		SDL_GameController* getSdlGameController() const;

		bool getButtonState(SDL_GameControllerButton button) const;

		// Value ranging from -1 and 1. Trigger however range from 0 to 1.
		float getAxisState(SDL_GameControllerAxis axis) const;

		bool isAttached() const;

		/// @brief Get the unique ID for the game controller for the time it is connected to the system,
		/// and is never reused for the lifetime of the application. If the joystick is
		///	disconnected and reconnected, it will get a new ID.
		/// 
		/// @return unique id for the game controller.
		SDL_JoystickID getInstanceId() const;

		/// @brief Get the stable unique GUID for the game controller
		/// @return GUID
		const SDL_JoystickGUID& getGuid() const;

	private:
		GameController(SDL_GameController* gameController, const SDL_JoystickGUID& guid) noexcept;

		SDL_GameController* gameController_ = nullptr;
		SDL_JoystickGUID guid_{};
	};

	inline SDL_GameController* GameController::getSdlGameController() const {
		return gameController_;
	}

	inline bool GameController::getButtonState(SDL_GameControllerButton button) const {
		return SDL_GameControllerGetButton(gameController_, button) != 0;
	}

	inline float GameController::getAxisState(SDL_GameControllerAxis axis) const {
		return static_cast<float>(SDL_GameControllerGetAxis(gameController_, axis) / sizeof(Sint16));
	}

	inline bool GameController::isAttached() const {
		return SDL_GameControllerGetAttached(gameController_) == SDL_TRUE;
	}

	inline SDL_JoystickID GameController::getInstanceId() const {
		return SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gameController_));
	}

}

#endif
