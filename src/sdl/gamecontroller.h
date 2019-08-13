#ifndef CPPSDL2_SDL_GAMECONTROLLER_H
#define CPPSDL2_SDL_GAMECONTROLLER_H

#include <string>

#include <SDL.h>

namespace sdl {

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

		bool operator==(SDL_JoystickID joystickId) const;

		const char* getName() const;

		// Get the underlaying pointer. Use with care.
		SDL_GameController* getSdlGameController() const;
		
		bool getButtonState(SDL_GameControllerButton button) const;

		// Value ranging from -1 and 1. Trigger however range from 0 to 1.
		double getAxisState(SDL_GameControllerAxis axis) const;

		bool isAttached() const;

		SDL_JoystickID getInstanceId() const;

	private:
		GameController(SDL_GameController* gameController) noexcept;

		SDL_GameController* gameController_ = nullptr;
	};

	inline SDL_GameController* GameController::getSdlGameController() const {
		return gameController_;
	}

	inline bool GameController::getButtonState(SDL_GameControllerButton button) const {
		return SDL_GameControllerGetButton(gameController_, button) != 0;
	}

	inline double GameController::getAxisState(SDL_GameControllerAxis axis) const {
		return (double) SDL_GameControllerGetAxis(gameController_, axis) / sizeof(Sint16);
	}

	inline bool GameController::isAttached() const {
		return SDL_GameControllerGetAttached(gameController_) == SDL_TRUE;
	}

	inline SDL_JoystickID GameController::getInstanceId() const {
		return SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gameController_));
	}

} // Namespace sdl.

#endif // CPPSDL2_SDL_GAMECONTROLLER_H
