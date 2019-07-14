#include "testwindow.h"
#include "testwindow2.h"

#if IMGUI_LIB
#include "testimguiwindow.h"
#endif // IMGUI_LIB
#include "types.h"

#include <sdl/sprite.h>
#include <sdl/sound.h>
#include <sdl/logger.h>
#include <sdl/textureatlas.h>
#include <sdl/initsdl.h>

#include <iostream>
#include <cassert>
#include <sstream>

SDL_Surface* createSurface(int w, int h, char r, char g, char b) {
	SDL_Surface* s = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	SDL_FillRect(s, 0, SDL_MapRGB(s->format, r, g, b));
	return s;
}

// Test to load directly to ram memory.
void testLoadTextureAtlas() {
	SDL_Surface* a = createSurface(200, 100, (char) 255, 0, 0); // Red block.
	SDL_Surface* b = createSurface(100, 200, 0, (char) 255, 0); // Green block.
	SDL_Surface* c = createSurface(200, 200, 0, 0, (char) 255); // Blue block.
	SDL_Surface* d = createSurface(30, 30, (char) 255, (char) 255, (char) 255); // White block.

	sdl::TextureAtlas atlas(512, 512);

	atlas.add("tetris.bmp", 1);
	atlas.add("cross.png", 1);
	atlas.add(a, 1);
	atlas.add(b, 1);
	atlas.add(c, 1);
	atlas.add(d, 1);

	TestWindow w(atlas.getTexture());
	w.startLoop();

	SDL_FreeSurface(a);
	SDL_FreeSurface(b);
	SDL_FreeSurface(c);
	SDL_FreeSurface(d);

	std::cout << "\ntestLoadTextureAtlas() successfully!\n";
}

// Test to load directly to graphic memory. And draw the total texture and the newly added sprite.
void testLoadTextureAtlas2() {
	SDL_Surface* a = createSurface(200, 100, (char) 255, 0, 0); // Red block.
	SDL_Surface* b = createSurface(100, 200, 0, (char) 255, 0); // Green block.
	SDL_Surface* c = createSurface(200, 200, 0, 0, (char) 255); // Blue block.
	SDL_Surface* d = createSurface(30, 30, (char) 255, (char) 255, (char) 255); // White block.

	sdl::TextureAtlas atlas(512, 512);
	TestWindow w(atlas.getTexture());
	int nbr = 0;
	std::function<void()> func = [&]() {
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
				sprite = atlas.add(a, 10);
				break;
			case 4:
				sprite = atlas.add(b, 5);
				break;
			case 5:
				sprite = atlas.add(c, 50);
				break;
			case 6:
				sprite = atlas.add(d, 20);
				break;
			default:
			{
				sprite = atlas.add(d, 1);
				break;
			}
		}
		if (!sprite.getTexture().isValid()) {
			std::cout << "\nNumber " << nbr << " failed to be inserted!\n";
		}
		w.setCenteredSprite(sprite);
	};

	w.setSpaceFunction(func);
	w.startLoop();

	SDL_FreeSurface(a);
	SDL_FreeSurface(b);
	SDL_FreeSurface(c);
	SDL_FreeSurface(d);

	std::cout << "\ntestLoadTextureAtlas2() successfully!\n";
}

void testBatchWindow() {
	// Test opengl versions.
	for (int i = 2; i <= 4; ++i) {
		try {
			TestWindow2 w(i, 1);
			std::stringstream stream;
			stream << "OpenGl Version " << i << "." << 1;
			w.setTitle(stream.str());
			w.startLoop();
		} catch (std::runtime_error& runtimeError) {
			std::cout << runtimeError.what();
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
	std::cout << "\t" << "-1                       testLoadTextureAtlas2" << "\n";
	std::cout << "\t" << "-1                       testBatchWindow" << "\n";
	std::cout << "\t" << "-1                       testImGuiWindow" << "\n";
}

void runAll() {
	testLoadTextureAtlas();
	testLoadTextureAtlas2();
	testBatchWindow();
}

int main(int argc, char** argv) {
	sdl::InitSdl sdl;
	   
	if (argc >= 2) {
		std::string programName = *argv;
		std::string code(*(argv + 1));
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
	}
	runAll();

	return 0;
}
