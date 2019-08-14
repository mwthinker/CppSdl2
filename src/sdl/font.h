#ifndef CPPSDL2_SDL_FONT_H
#define CPPSDL2_SDL_FONT_H

#include <string>
#include <memory>

#include <SDL_ttf.h>

namespace sdl {

	class Font {
	public:
		Font() noexcept;

		Font(const Font& font) = default;
		Font& operator=(const Font& font) = default;

		Font(Font&& font) noexcept;
		Font& operator=(Font&& font) noexcept;

		bool operator==(const Font& font) const noexcept;

		bool operator!=(const Font& font) const noexcept;

		Font(const std::string& filename, int characterSize);

		int getCharacterSize() const noexcept;

		// Use with care! Return a pointer to the font data.
		TTF_Font* getTtfFont() const noexcept;

	private:
		struct FontData {
			FontData(TTF_Font* font) noexcept;
			~FontData();

			TTF_Font* font_ = nullptr;
		};

		std::shared_ptr<FontData> fontData_;
		int characterSize_ = 0;
	};

	inline bool Font::operator==(const Font& font) const noexcept {
		return fontData_ == font.fontData_;
	}

	inline bool Font::operator!=(const Font& other) const noexcept {
		return !(*this == other);
	}

	inline int Font::getCharacterSize() const noexcept {
		return characterSize_;
	}

} // Namespace sdl.

#endif // CPPSDL2_SDL_FONT_H
