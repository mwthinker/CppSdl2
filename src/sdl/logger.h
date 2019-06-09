#ifndef CPPSDL2_SDL_LOGGER_H
#define CPPSDL2_SDL_LOGGER_H

#include <spdlog/spdlog.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h> 

namespace sdl {
	
	static std::shared_ptr<spdlog::logger> logger(const char* name = "sdl") {
		auto logger = spdlog::get(name);
		if (logger == nullptr) {
			logger = spdlog::stdout_color_mt(name);
		}
		return logger;
	}

} // Namespace sdl.

#endif // CPPSDL2_SDL_SPRITE_H
