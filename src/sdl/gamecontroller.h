#ifndef CPPSDL2_SDL_GAMECONTROLLER_H
#define CPPSDL2_SDL_GAMECONTROLLER_H

#include "initsdl.h"

#include <memory>
#include <string>
#include <vector>

namespace sdl {

	class GameController;
	typedef std::shared_ptr<GameController> GameControllerPtr;

	class GameController : public InitSdl {
	public:
		~GameController();

		GameController(const GameController&) = delete;
		GameController& operator=(const GameController&) = delete;

		// Return the name of the joystick.
		std::string getName() const;

		// Return all gamepads. A window must bee active.
		static const std::vector<GameControllerPtr>& getGameControllers();

		static void loadAddGameControllerMappings(const std::string& file);

		static GameControllerPtr addController(int index);
		static GameControllerPtr removeController(SDL_JoystickID instanceId);

		// Get the underlaying pointer. Use with care.
		SDL_GameController* getSdlGameController() const;
		
		// Return the button state.
		bool getButtonState(SDL_GameControllerButton button) const;

		// Return the axis state. Value ranging from -1 and 1. Trigger however range from 0 to 1.
		double getAxisState(SDL_GameControllerAxis axis) const;

		bool isAttached() const;

		SDL_JoystickID getInstanceId() const;

	private:
		GameController(SDL_GameController* gameController);

		SDL_GameController* gameController_;
		static std::vector<GameControllerPtr> gameControllers;
	};
	

	inline SDL_GameController* GameController::getSdlGameController() const {
		return gameController_;
	}

	// Return the button state.
	inline bool GameController::getButtonState(SDL_GameControllerButton button) const {
		return SDL_GameControllerGetButton(gameController_, button) != 0;
	}

	// Return the axis state. Value ranging from -1 and 1. Trigger however range from 0 to 1.
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
