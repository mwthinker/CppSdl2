#include "font.h"
#include "logger.h"

#include <string>

#include <SDL_ttf.h>

namespace sdl {

	Font::~Font() {
		if (font_ != nullptr) {
			TTF_CloseFont(font_);
		}
	}

	Font::Font(const std::string& filename, int characterSize) {
		font_ = TTF_OpenFont(filename.c_str(), characterSize);

		if (font_ != nullptr) {
			characterSize_ = characterSize;
			TTF_SetFontHinting(font_, TTF_HINTING_LIGHT);
		} else {
			logger()->error("[Font] Error font: {}", TTF_GetError());
		}
	}

	Font::Font(Font&& other) noexcept : font_{other.font_}, 
		characterSize_{other.characterSize_} {

		other.font_ = nullptr;
		other.characterSize_ = 0;
	}

	Font& Font::operator=(Font&& other) noexcept {
		font_ = other.font_;
		characterSize_ = other.characterSize_;

		other.font_ = nullptr;
		other.characterSize_ = 0;
		return *this;
	}

	bool Font::operator==(const Font& font) const noexcept {
		return font_ == font.font_;
	}

	bool Font::operator!=(const Font& other) const noexcept {
		return font_ != other.font_;
	}

	int Font::getCharacterSize() const noexcept {
		return characterSize_;
	}

	bool Font::isLoaded() const noexcept {
		return font_ != nullptr;
	}

} // Namespace sdl.
