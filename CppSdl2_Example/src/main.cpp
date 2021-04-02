#include "testwindow.h"
#include "graphicwindow.h"

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

using namespace sdl::color;

struct HtmlColor {
	std::string name;
	sdl::Color color;
};

#define HTML_COLOR(name) {#name, sdl::color::html::name}

std::vector<HtmlColor> getHtmlColors() {
	std::vector<HtmlColor> htmlColors;
	htmlColors.push_back(HTML_COLOR(MediumVioletRed));
	htmlColors.push_back(HTML_COLOR(DeepPink));
	htmlColors.push_back(HTML_COLOR(PaleVioletRed));
	htmlColors.push_back(HTML_COLOR(HotPink));
	htmlColors.push_back(HTML_COLOR(LightPink));
	htmlColors.push_back(HTML_COLOR(Pink));
	htmlColors.push_back(HTML_COLOR(DarkRed));
	htmlColors.push_back(HTML_COLOR(Red));
	htmlColors.push_back(HTML_COLOR(Firebrick));
	htmlColors.push_back(HTML_COLOR(Crimson));
	htmlColors.push_back(HTML_COLOR(IndianRed));
	htmlColors.push_back(HTML_COLOR(LightCoral));
	htmlColors.push_back(HTML_COLOR(Salmon));
	htmlColors.push_back(HTML_COLOR(DarkSalmon));
	htmlColors.push_back(HTML_COLOR(LightSalmon));
	htmlColors.push_back(HTML_COLOR(OrangeRed));
	htmlColors.push_back(HTML_COLOR(Tomato));
	htmlColors.push_back(HTML_COLOR(DarkOrange));
	htmlColors.push_back(HTML_COLOR(Coral));
	htmlColors.push_back(HTML_COLOR(Orange));
	htmlColors.push_back(HTML_COLOR(DarkKhaki));
	htmlColors.push_back(HTML_COLOR(Gold));
	htmlColors.push_back(HTML_COLOR(Khaki));
	htmlColors.push_back(HTML_COLOR(PeachPuff));
	htmlColors.push_back(HTML_COLOR(Yellow));
	htmlColors.push_back(HTML_COLOR(PaleGoldenrod));
	htmlColors.push_back(HTML_COLOR(Moccasin));
	htmlColors.push_back(HTML_COLOR(PapayaWhip));
	htmlColors.push_back(HTML_COLOR(LightGoldenrodYellow));
	htmlColors.push_back(HTML_COLOR(LemonChiffon));
	htmlColors.push_back(HTML_COLOR(LightYellow));
	htmlColors.push_back(HTML_COLOR(Maroon));
	htmlColors.push_back(HTML_COLOR(Brown));
	htmlColors.push_back(HTML_COLOR(SaddleBrown));
	htmlColors.push_back(HTML_COLOR(Sienna));
	htmlColors.push_back(HTML_COLOR(Chocolate));
	htmlColors.push_back(HTML_COLOR(DarkGoldenrod));
	htmlColors.push_back(HTML_COLOR(Peru));
	htmlColors.push_back(HTML_COLOR(RosyBrown));
	htmlColors.push_back(HTML_COLOR(Goldenrod));
	htmlColors.push_back(HTML_COLOR(SandyBrown));
	htmlColors.push_back(HTML_COLOR(Tan));
	htmlColors.push_back(HTML_COLOR(Burlywood));
	htmlColors.push_back(HTML_COLOR(Wheat));
	htmlColors.push_back(HTML_COLOR(NavajoWhite));
	htmlColors.push_back(HTML_COLOR(Bisque));
	htmlColors.push_back(HTML_COLOR(BlanchedAlmond));
	htmlColors.push_back(HTML_COLOR(Cornsilk));
	htmlColors.push_back(HTML_COLOR(DarkGreen));
	htmlColors.push_back(HTML_COLOR(Green));
	htmlColors.push_back(HTML_COLOR(DarkOliveGreen));
	htmlColors.push_back(HTML_COLOR(ForestGreen));
	htmlColors.push_back(HTML_COLOR(SeaGreen));
	htmlColors.push_back(HTML_COLOR(Olive));
	htmlColors.push_back(HTML_COLOR(OliveDrab));
	htmlColors.push_back(HTML_COLOR(MediumSeaGreen));
	htmlColors.push_back(HTML_COLOR(LimeGreen));
	htmlColors.push_back(HTML_COLOR(Lime));
	htmlColors.push_back(HTML_COLOR(SpringGreen));
	htmlColors.push_back(HTML_COLOR(MediumSpringGreen));
	htmlColors.push_back(HTML_COLOR(DarkSeaGreen));
	htmlColors.push_back(HTML_COLOR(MediumAquamarine));
	htmlColors.push_back(HTML_COLOR(YellowGreen));
	htmlColors.push_back(HTML_COLOR(LawnGreen));
	htmlColors.push_back(HTML_COLOR(Chartreuse));
	htmlColors.push_back(HTML_COLOR(LightGreen));
	htmlColors.push_back(HTML_COLOR(GreenYellow));
	htmlColors.push_back(HTML_COLOR(PaleGreen));
	htmlColors.push_back(HTML_COLOR(Teal));
	htmlColors.push_back(HTML_COLOR(DarkCyan));
	htmlColors.push_back(HTML_COLOR(LightSeaGreen));
	htmlColors.push_back(HTML_COLOR(CadetBlue));
	htmlColors.push_back(HTML_COLOR(DarkTurquoise));
	htmlColors.push_back(HTML_COLOR(MediumTurquoise));
	htmlColors.push_back(HTML_COLOR(Turquoise));
	htmlColors.push_back(HTML_COLOR(Aqua));
	htmlColors.push_back(HTML_COLOR(Cyan));
	htmlColors.push_back(HTML_COLOR(Aquamarine));
	htmlColors.push_back(HTML_COLOR(PaleTurquoise));
	htmlColors.push_back(HTML_COLOR(LightCyan));
	htmlColors.push_back(HTML_COLOR(Navy));
	htmlColors.push_back(HTML_COLOR(DarkBlue));
	htmlColors.push_back(HTML_COLOR(MediumBlue));
	htmlColors.push_back(HTML_COLOR(Blue));
	htmlColors.push_back(HTML_COLOR(MidnightBlue));
	htmlColors.push_back(HTML_COLOR(RoyalBlue));
	htmlColors.push_back(HTML_COLOR(SteelBlue));
	htmlColors.push_back(HTML_COLOR(DodgerBlue));
	htmlColors.push_back(HTML_COLOR(DeepSkyBlue));
	htmlColors.push_back(HTML_COLOR(CornflowerBlue));
	htmlColors.push_back(HTML_COLOR(SkyBlue));
	htmlColors.push_back(HTML_COLOR(LightSkyBlue));
	htmlColors.push_back(HTML_COLOR(LightSteelBlue));
	htmlColors.push_back(HTML_COLOR(LightBlue));
	htmlColors.push_back(HTML_COLOR(PowderBlue));
	htmlColors.push_back(HTML_COLOR(Indigo));
	htmlColors.push_back(HTML_COLOR(Purple));
	htmlColors.push_back(HTML_COLOR(DarkMagenta));
	htmlColors.push_back(HTML_COLOR(DarkViolet));
	htmlColors.push_back(HTML_COLOR(DarkSlateBlue));
	htmlColors.push_back(HTML_COLOR(BlueViolet));
	htmlColors.push_back(HTML_COLOR(DarkOrchid));
	htmlColors.push_back(HTML_COLOR(Fuchsia));
	htmlColors.push_back(HTML_COLOR(Magenta));
	htmlColors.push_back(HTML_COLOR(SlateBlue));
	htmlColors.push_back(HTML_COLOR(MediumSlateBlue));
	htmlColors.push_back(HTML_COLOR(MediumOrchid));
	htmlColors.push_back(HTML_COLOR(MediumPurple));
	htmlColors.push_back(HTML_COLOR(Orchid));
	htmlColors.push_back(HTML_COLOR(Violet));
	htmlColors.push_back(HTML_COLOR(Plum));
	htmlColors.push_back(HTML_COLOR(Thistle));
	htmlColors.push_back(HTML_COLOR(Lavender));
	htmlColors.push_back(HTML_COLOR(MistyRose));
	htmlColors.push_back(HTML_COLOR(AntiqueWhite));
	htmlColors.push_back(HTML_COLOR(Linen));
	htmlColors.push_back(HTML_COLOR(Beige));
	htmlColors.push_back(HTML_COLOR(WhiteSmoke));
	htmlColors.push_back(HTML_COLOR(LavenderBlush));
	htmlColors.push_back(HTML_COLOR(OldLace));
	htmlColors.push_back(HTML_COLOR(AliceBlue));
	htmlColors.push_back(HTML_COLOR(Seashell));
	htmlColors.push_back(HTML_COLOR(GhostWhite));
	htmlColors.push_back(HTML_COLOR(Honeydew));
	htmlColors.push_back(HTML_COLOR(FloralWhite));
	htmlColors.push_back(HTML_COLOR(Azure));
	htmlColors.push_back(HTML_COLOR(MintCream));
	htmlColors.push_back(HTML_COLOR(Snow));
	htmlColors.push_back(HTML_COLOR(Ivory));
	htmlColors.push_back(HTML_COLOR(White));
	htmlColors.push_back(HTML_COLOR(Black));
	htmlColors.push_back(HTML_COLOR(DarkSlateGray));
	htmlColors.push_back(HTML_COLOR(DimGray));
	htmlColors.push_back(HTML_COLOR(SlateGray));
	htmlColors.push_back(HTML_COLOR(Gray));
	htmlColors.push_back(HTML_COLOR(LightSlateGray));
	htmlColors.push_back(HTML_COLOR(DarkGray));
	htmlColors.push_back(HTML_COLOR(Silver));
	htmlColors.push_back(HTML_COLOR(LightGray));
	htmlColors.push_back(HTML_COLOR(Gainsboro));
	return htmlColors;
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

	static auto htmlColors = getHtmlColors();
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
				if (++index < htmlColors.size()) {
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
			testGraphicWindow();
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
