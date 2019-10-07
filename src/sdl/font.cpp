#include "font.h"
#include "logger.h"

#include <string>

#include <SDL_ttf.h>

namespace sdl {

	Font::Font() noexcept : characterSize_{0} {
	}

	Font::Font(Font&& font) noexcept : fontData_{std::move(font.fontData_)}, characterSize_{font.characterSize_} {
	}
	
	Font& Font::operator=(Font&& font) noexcept {
		if (font.fontData_) {
			fontData_ = std::move(fontData_);
			characterSize_ = font.characterSize_;
			font.characterSize_ = 0;
		}
		return *this;
	}

	Font::Font(const std::string& filename, int characterSize) :
	    fontData_(nullptr), characterSize_(0) {

		if (TTF_Font* font = TTF_OpenFont(filename.c_str(), characterSize);
			font != nullptr) {
			
			fontData_ = std::make_shared<FontData>(font);
			characterSize_ = characterSize;
			TTF_SetFontHinting(font, TTF_HINTING_LIGHT);
		} else {
			logger()->error("[Font] Error font: {}", TTF_GetError());
		}
	}

	TTF_Font* Font::getTtfFont() const noexcept {
		return fontData_ != nullptr ? fontData_->font : nullptr;
	}

	Font::FontData::FontData(TTF_Font* font) noexcept : font(font) {
	}

	Font::FontData::~FontData() {
		if (font != 0) {
			TTF_CloseFont(font);
		}
	}

} // Namespace sdl.
