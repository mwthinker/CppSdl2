#include "logger.h"

namespace sdl {
	
	std::shared_ptr<spdlog::logger> logger(const char* name) {
		auto logger = spdlog::get(name);
		if (logger == nullptr) {
			logger = spdlog::stdout_color_mt(name);
		}
		return logger;
	}

} // Namespace sdl.
