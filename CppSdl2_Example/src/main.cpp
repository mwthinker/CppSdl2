#include "testwindow.h"
#include "testwindow2.h"

#include "testimguiwindow.h"
#include "types.h"

#include <sdl/sprite.h>
#include <sdl/sound.h>
#include <sdl/textureatlas.h>
#include <sdl/initsdl.h>

#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <iostream>
#include <cassert>
#include <sstream>

// Test to load directly to ram memory.
void testLoadTextureAtlas() {
	sdl::Surface red{200, 100, sdl::Red};
	sdl::Surface green{100, 200, sdl::Green};
	sdl::Surface blue{200, 200, sdl::Blue};
	sdl::Surface white{30, 30, sdl::White};

	sdl::TextureAtlas atlas{512, 512};

	sdl::Sprite s = atlas.add("tetris.bmp", 1);
	s = atlas.add("cross.png", 1);
	s = atlas.add(red, 1);
	s = atlas.add(green, 1);
	s = atlas.add(blue, 1);
	s = atlas.add(white, 1);

	TestWindow w{atlas.get()};
	w.startLoop();

	spdlog::info("[testLoadTextureAtlas] Successfully!");
}

// Test to load directly to graphic memory. And draw the total texture and the newly added sprite.
void testLoadTextureAtlas2() {
	sdl::Surface red{200, 100, sdl::Red};
	sdl::Surface green{100, 200, sdl::Green};
	sdl::Surface blue{200, 200, sdl::Blue};
	sdl::Surface white{30, 30, sdl::White};

	sdl::TextureAtlas atlas{512, 512};
	TestWindow w{atlas.get()};
	int nbr = 0;
	auto func = [&]() {
		++nbr;
		sdl::Sprite sprite;
		switch (nbr) {
			case 1:
				sprite = atlas.add("tetris.bmp", 1);
				break;
			case 2:
				sprite = atlas.add("cross.png", 1);
				break;
			case 3:
				sprite = atlas.add(red, 10);
				break;
			case 4:
				sprite = atlas.add(green, 5);
				break;
			case 5:
				sprite = atlas.add(blue, 50);
				break;
			case 6:
				sprite = atlas.add(white, 20);
				break;
			default:
				sprite = atlas.add(white, 1);
				break;
		}
		if (!sprite.isValid()) {
			spdlog::info("[testLoadTextureAtlas2] Number {} failed to be inserted!", nbr);
		}
	};

	w.setSpaceFunction(func);
	w.startLoop();

	spdlog::info("[testLoadTextureAtlas2] Successfully!");
}

void testBatchWindow() {
	try {
		TestWindow2 w{3, 3};
		std::string glVersion{"OpenGl Version 3.3"};
		spdlog::info("[testBatchWindow] {}", glVersion);
		w.setTitle(glVersion);
		w.startLoop();
	} catch (std::runtime_error& runtimeError) {
		spdlog::error("[testBatchWindow] Runtime exception: {}", runtimeError.what());
	}
}

void testImGuiWindow() {
	TestImGuiWindow w;
	w.startLoop();
}

void showHelp(const std::string& programName) {
	std::cout << "Usage: " << programName << "\n";	
	std::cout << "\t" << programName << " -1  " << "\n";
	std::cout << "\t" << programName << " -2  " << "\n";
	std::cout << "\t" << programName << " -3  " << "\n";
	std::cout << "\t" << programName << " -4  " << "\n";
	std::cout << "\n";
	std::cout << "Options:\n";
	std::cout << "\t" << "-h --help                show this help" << "\n";
	std::cout << "\t" << "-1                       testLoadTextureAtlas" << "\n";
	std::cout << "\t" << "-2                       testLoadTextureAtlas2" << "\n";
	std::cout << "\t" << "-3                       testBatchWindow" << "\n";
	std::cout << "\t" << "-4                       testImGuiWindow" << "\n";
}

void runAll() {
	testLoadTextureAtlas();
	testLoadTextureAtlas2();
	testBatchWindow();
	testImGuiWindow();
}

void initLogger() {
	try {
		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		console_sink->set_level(spdlog::level::warn);
		console_sink->set_pattern("[multi_sink_example] [%^%l%$] %v");

		auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/multisink.txt", true);
		file_sink->set_level(spdlog::level::trace);

		spdlog::sinks_init_list sink_list = {file_sink, console_sink};
		//spdlog::flush_every(std::chrono::seconds{5});

		spdlog::logger logger{"multi_sink_example", sink_list.begin(), sink_list.end()};
		logger.set_level(spdlog::level::debug);
		logger.warn("this should appear in both console and file");
		logger.info("this message should not appear in the console, only in the file");
		logger.error("error text");

		// or you can even set multi_sink logger as default logger
		spdlog::set_default_logger(std::make_shared<spdlog::logger>("multi_sink", spdlog::sinks_init_list({console_sink, file_sink})));
	} catch (const spdlog::spdlog_ex& ex) {
		std::cout << "Log initialization failed: " << ex.what() << std::endl;
	}
}

int main(int argc, char** argv) {
	initLogger();

	sdl::InitSdl sdl{SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER};

	if (argc >= 2) {
		std::string programName = *argv;
		std::string code{*(argv + 1)};
		if (code == "-h" || code == "--help") {
			showHelp(programName);
			return 0;
		} else if (code == "-1") {
			testLoadTextureAtlas();
			return 0;
		} else if (code == "-2") {
			testLoadTextureAtlas2();
			return 0;
		} else if (code == "-3") {
			testBatchWindow();
			return 0;
		} else if (code == "-4") {
			testImGuiWindow();
			return 0;			
		} else {
			std::cout << "Incorrect argument " << code << "\n";
		}
	} else {
		runAll();
	}

	return 0;
}
