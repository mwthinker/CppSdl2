#include "testwindow.h"
#include "graphicwindow.h"

#include "testimguiwindow.h"
#include "types.h"

#include <sdl/sprite.h>
#include <sdl/sound.h>
#include <sdl/textureatlas.h>
#include <sdl/initsdl.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <fmt/core.h>

#include <cassert>
#include <sstream>

using namespace sdl::color;

void testPrintColors() {
	for (const auto& [name, color] : html::getHtmlColors()) {
		spdlog::info("{}: {}", name, color.toHexString());
	}
}

// Test to load directly to ram memory.
void testLoadTextureAtlas() {
	sdl::Surface red{200, 100, Red};
	sdl::Surface green{100, 200, Green};
	sdl::Surface blue{200, 200, Blue};
	sdl::Surface white{30, 30, White};

	sdl::TextureAtlas atlas{512, 512};

	sdl::Sprite s = atlas.add("tetris.bmp", 1);
	s = atlas.add("cross.png", 1);
	s = atlas.add(red, 1);
	s = atlas.add(green, 1);
	s = atlas.add(blue, 1);
	s = atlas.add(white, 1);

	TestWindow w{atlas.get()};
	w.setBordered(false);
	w.setAlwaysOnTop(true);
	w.setOpacity(0.5f);
	w.startLoop();

	spdlog::info("[testLoadTextureAtlas] Successfully!");
}

// Test to load directly to graphic memory. And draw the total texture and the newly added sprite.
void testLoadTextureAtlas2() {
	sdl::Surface red{200, 100, Red};
	sdl::Surface green{100, 200, Green};
	sdl::Surface blue{200, 200, Blue};
	sdl::Surface white{30, 30, White};

	static auto htmlColors = html::getHtmlColors();
	static int index = -1;

	sdl::TextureAtlas atlas{600, 600};
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
			default:
				if (++index < static_cast<int>(htmlColors.size())) {
					auto html = htmlColors[index];
					sprite = atlas.add(sdl::Surface{30, 30, html.color}, 1);
					spdlog::info("[testLoadTextureAtlas2] Color added {} = {} ", html.name, html.color.toHexString());
				} else {
					sprite = atlas.add(white, 1);
					spdlog::info("[testLoadTextureAtlas2] Add white");
				}
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

void testGraphicWindow() {
	try {
		GraphicWindow w{3, 3};
		std::string glVersion{"OpenGl Version 3.3"};
		spdlog::info("[testGraphicWindow] {}", glVersion);
		w.setTitle(glVersion);
		w.startLoop();
	} catch (const std::runtime_error& runtimeError) {
		spdlog::error("[testGraphicWindow] Runtime exception: {}", runtimeError.what());
	}
}

void testImGuiWindow() {
	TestImGuiWindow w;
	w.startLoop();
}

void showHelp(const std::string& programName) {
	fmt::println("Usage: {}", programName);
	fmt::println("\t{} -1 ", programName);
	fmt::println("\t{} -2 ", programName);
	fmt::println("\t{} -3 ", programName);
	fmt::println("\t{} -4 ", programName);
	fmt::println("");
	fmt::println("Options:");
	fmt::println("\t-h --help                show this help");
	fmt::println("\t-1                       testPrintColors");
	fmt::println("\t-2                       testLoadTextureAtlas");
	fmt::println("\t-3                       testLoadTextureAtlas2");
	fmt::println("\t-4                       testBatchWindow");
	fmt::println("\t-5                       testImGuiWindow");
}

void runAll() {
	testPrintColors();
	testLoadTextureAtlas();
	testLoadTextureAtlas2();
	testGraphicWindow();
	testImGuiWindow();
}

void initLogger() {
	try {
		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		console_sink->set_level(spdlog::level::debug);
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
		fmt::println("Log initialization failed: {}", ex.what());
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
			testPrintColors();
		} else if (code == "-2") {
			testLoadTextureAtlas();
			return 0;
		} else if (code == "-3") {
			testLoadTextureAtlas2();
			return 0;
		} else if (code == "-4") {
			testGraphicWindow();
			return 0;
		} else if (code == "-5") {
			testImGuiWindow();
			return 0;
		} else {
			fmt::println("Incorrect argument {}", code);
		}
	} else {
		runAll();
	}

	return 0;
}
