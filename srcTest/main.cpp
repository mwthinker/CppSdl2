#include "testwindow.h"
#include "testwindow2.h"

#if IMGUI_LIB
#include "testimguiwindow.h"
#endif // IMGUI_LIB
#include "types.h"
#include "logger.h"

#include <sdl/sprite.h>
#include <sdl/sound.h>
#include <sdl/logger.h>
#include <sdl/textureatlas.h>
#include <sdl/initsdl.h>

#include <iostream>
#include <cassert>
#include <sstream>

// Test to load directly to ram memory.
void testLoadTextureAtlas() {
	sdl::Surface red{200, 100, sdl::RED};
	sdl::Surface green{100, 200, sdl::GREEN};
	sdl::Surface blue{200, 200, sdl::BLUE};
	sdl::Surface white{30, 30, sdl::WHITE};

	sdl::TextureAtlas atlas{512, 512};

	sdl::Sprite s = atlas.add("tetris.bmp", 1);
	s = atlas.add("cross.png", 1);
	s = atlas.add(red, 1);
	s = atlas.add(green, 1);
	s = atlas.add(blue, 1);
	s = atlas.add(white, 1);

	TestWindow w{atlas.get()};
	w.startLoop();

	logger()->info("[testLoadTextureAtlas] Successfully!");
}

// Test to load directly to graphic memory. And draw the total texture and the newly added sprite.
void testLoadTextureAtlas2() {
	sdl::Surface red{200, 100, sdl::RED};
	sdl::Surface green{100, 200, sdl::GREEN};
	sdl::Surface blue{200, 200, sdl::BLUE};
	sdl::Surface white{30, 30, sdl::WHITE};

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
			{
				sprite = atlas.add(white, 1);
				break;
			}
		}
		if (!sprite.isValid()) {
			logger()->warn("[testLoadTextureAtlas2] Number {} failed to be inserted!", nbr);
		}
	};

	w.setSpaceFunction(func);
	w.startLoop();

	logger()->info("[testLoadTextureAtlas2] Successfully!");
}

void testBatchWindow() {
	// Test opengl versions.
	for (int i = 2; i <= 4; ++i) {
		try {
			TestWindow2 w{i, 1};
			std::stringstream stream;
			stream << "OpenGl Version " << i << "." << 1;
			logger()->info("[testBatchWindow] ", stream.str());
			w.setTitle(stream.str());
			w.startLoop();
		} catch (std::runtime_error& runtimeError) {
			logger()->error("[testBatchWindow] Runtime exception: {}", runtimeError.what());
		}
	}
}

void testImGuiWindow() {
#if IMGUI_LIB
	TestImGuiWindow w;
	w.startLoop();
#endif // IMGUI_LIB
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
#if IMGUI_LIB
	std::cout << "\t" << "-4                       testImGuiWindow" << "\n";
#endif // IMGUI_LIB
}

void runAll() {
	testLoadTextureAtlas();
	testLoadTextureAtlas2();
	testBatchWindow();
	testImGuiWindow();
}

int main(int argc, char** argv) {
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
#if IMGUI_LIB
			testImGuiWindow();
			return 0;
#else
			std::cout << "ImGui not compiled, must add preprocessor IMGUI_LIB\n";
#endif // IMGUI_LIB
			
		} else {
			std::cout << "Incorrect argument " << code << "\n";
		}
	} else {
		runAll();
	}

	return 0;
}
