#include "text.h"
#include "font.h"
#include "sprite.h"
#include "opengl.h"

#include <SDL_ttf.h>

namespace sdl {

	Text::Text(const std::string& text, const Font& font) : characterSize_((float) font.getCharacterSize()), font_(font) {
		setText(text);
	}

	Text::Text(const std::string& text, const Font& font, float characterSize) : characterSize_(characterSize), font_(font) {
	    setText(text);
	}

	Text::Text(Text&& other) noexcept :
		font_(std::move(other.font_)), texture_(std::move(other.texture_)),
		text_(std::move(other.text_)), characterSize_(other.characterSize_) {

		other.characterSize_ = 0;
	}

	Text& Text::operator=(Text&& other) noexcept {
		font_ = std::move(other.font_);
		texture_ = std::move(other.texture_);
		text_ = std::move(other.text_);
		characterSize_ = other.characterSize_;

		other.characterSize_ = 0;
		return *this;
	}

	void Text::setText(const std::string& text) {
		// Text changed?
		if (text_ != text) {
		    // Try to load the text to a new texture.
            text_ = text;
			if (!loadTextToTexture(text)) {
			    // Texture loading failed.
			    texture_ = Texture();
			}
		}
	}

	bool Text::loadTextToTexture(const std::string& str) {
	    // Font available?
		if (font_.getTtfFont()) {
			if (str.size() > 0) {
				SDL_Color color = {255, 255, 255, 255};
				SDL_Surface* surface = TTF_RenderUTF8_Blended_Wrapped(font_.getTtfFont(), str.c_str(), color, 500);
				
				// No error?
				if (surface != 0) {
                    // Texture takes ownership of surface.
                    texture_ = Texture(surface, []() {
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                    });
                    return true;
				}
			}
		}
		return false;
	}

} // Namespace sdl.
