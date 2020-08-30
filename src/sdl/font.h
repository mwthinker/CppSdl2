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

		bool operator==(const Font& font) const noexcept;

		bool operator!=(const Font& font) const noexcept;

		int getCharacterSize() const noexcept;

		bool isLoaded()  const noexcept;

	private:
		friend class Surface;

		TTF_Font* font_{};
		int characterSize_{};
	};

}

#endif
