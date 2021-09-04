#ifndef CPPSDL2_SDL_FONT_H
#define CPPSDL2_SDL_FONT_H

#include <string>

#include <SDL_ttf.h>

namespace sdl {

	class Font {
	public:
		Font() noexcept = default;
		~Font();

		Font(const std::string& filename, int characterSize);

		Font(const Font&) = delete;
		Font& operator=(const Font&) = delete;

		Font(Font&& other) noexcept;
		Font& operator=(Font&& other) noexcept;

		friend bool operator==(const Font& left, const Font& right) noexcept;

		friend bool operator!=(const Font& left, const Font& right) noexcept;

		int getCharacterSize() const noexcept;

		bool isLoaded()  const noexcept;

	private:
		friend class Surface;

		TTF_Font* font_ = nullptr;
		int characterSize_= 0;
	};

}

#endif
