#include "text.h"
#include "font.h"
#include "sprite.h"
#include "opengl.h"
#include "surface.h"
#include "texture.h"

#include <SDL_ttf.h>

namespace sdl {

	Text::Text(const std::string& text, const Font& font)
		: characterSize_{font.getCharacterSize()} {
		
		// Font available?
		if (font.isLoaded() && !text.empty()) {
			Surface surface{text, font, {255, 255, 255, 255}};

			if (surface.isLoaded()) {
				sprite_ = Sprite(std::move(surface), []() {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				});
			}
		}
	}

	const Sprite& Text::getSprite() const noexcept {
		return sprite_;
	}
	
	float Text::getWidth() const noexcept {
		return sprite_.getWidth();
	}
	
	float Text::getHeight() const noexcept {
		return sprite_.getHeight();
	}	

	void Text::bindTexture() const {
		return sprite_.bindTexture();
	}

	bool Text::isValid() const noexcept {
		return sprite_.isValid();
	}

	bool Text::operator==(const Text& text) const noexcept {
		return Sprite::equalSource(sprite_, text.sprite_);
	}

	bool Text::operator!=(const Text& other) const noexcept {
		return !(*this == other);
	}

} // Namespace sdl.
