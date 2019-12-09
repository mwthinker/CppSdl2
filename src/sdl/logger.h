#ifndef CPPSDL2_SDL_LOGGER_H
#define CPPSDL2_SDL_LOGGER_H

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace sdl {
	
	std::shared_ptr<spdlog::logger> logger(const char* name = "sdl");

} // Namespace sdl.

#endif // CPPSDL2_SDL_LOGGER_H
