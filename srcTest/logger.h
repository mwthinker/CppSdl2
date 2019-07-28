#ifndef LOGGER_H
#define LOGGER_H

#include <sdl/logger.h>
	
static std::shared_ptr<spdlog::logger> logger() {
	return sdl::logger("CppSdl2Test");
}

#endif // LOGGER_H
