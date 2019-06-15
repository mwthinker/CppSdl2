#ifndef CPPSDL2_SDL_TEXT_H
#define CPPSDL2_SDL_TEXT_H

#include "font.h"
#include "texture.h"

#include <string>

namespace sdl {

	class Text {
	public:
		// Create a empty text object.
		Text();

		// Create a Text object with the text and font provided.
		// The character size for the text is the same as the font.
		Text(const std::string& text, const Font& font);

		// Create a Text object with the text,font and character size provided.
		// The text texture is created in size provided by the font but then
		// scaled to match the provided character size.
		Text(const std::string& text, const Font& font, float characterSize);

		Text(const Text&) = default;
		Text& operator=(const Text&) = default;

		Text(Text&& other) noexcept;
		Text& operator=(Text&& other) noexcept;

		bool operator==(const Text& text) const;

		bool operator!=(const Text& text) const;

		void setText(const std::string& text);

		std::string getText() const;

		// Set the character size. The text texture is created in the size
		// provided by the font but then scaled to match the
		// provided character size.
		void setCharacterSize(float characterSize);

		// Return the character size used for the text. May
		// or may not correspond to the internal font size.
		float getCharacterSize() const;

		// Returns the width of the text to be drawn.
		// (The width in openGL).
		float getWidth() const;

		// Return the height of the text to be drawn.
		// (The height in openGL).
		float getHeight() const;

		// Return the current font. The font may be empty.
		const Font& getFont() const;

        // Return the texture representing the text. The texture may be empty.
		const Texture& getTexture() const;

		void bindTexture() const;

        // Return if the underlying texture is valid.
        // I.e. the texture is in memory with width and height.
		bool isValid() const;

	private:
		bool loadTextToTexture(const std::string& str);

        Font font_;
		Texture texture_;
		std::string text_;
		float characterSize_;
	};

	inline std::string Text::getText() const {
		return text_;
	}

	// Set the character size. The text texture is created in the size
	// provided by the font but then scaled to match the
	// provided character size.
	inline void Text::setCharacterSize(float characterSize) {
		characterSize_ = characterSize;
	}

	// Return the character size used for the text. May
	// or may not correspond to the internal font size.
	inline float Text::getCharacterSize() const {
		return characterSize_;
	}

	// Returns the width of the text to be drawn.
	// (The width in openGL).
	inline float Text::getWidth() const {
		if (font_.getCharacterSize() < 1) {
			return 0;
		}
		return (float) (texture_.getWidth() * characterSize_ / font_.getCharacterSize());
	}

	// Return the height of the text to be drawn.
	// (The height in openGL).
	inline float Text::getHeight() const {
		if (font_.getCharacterSize() < 1) {
			return 0;
		}
		return (float) (texture_.getHeight() * characterSize_ / font_.getCharacterSize());
	}

	// Return the current font. The font may be empty.
	inline const Font& Text::getFont() const {
		return font_;
	}

    // Return the texture representing the text. The texture may be empty.
	inline const Texture& Text::getTexture() const {
		return texture_;
	}

	inline void Text::bindTexture() const {
        return texture_.bindTexture();
    }

    // Return if the underlying texture is valid.
    // I.e. the texture is in memory with width and height.
	inline bool Text::isValid() const {
        return texture_.isValid();
	}

	inline bool Text::operator==(const Text& text) const {
		return texture_ == text.texture_;
	}

	inline bool Text::operator!=(const Text& other) const {
		return !(*this == other);
	}

} // Namespace sdl.

#endif // CPPSDL2_SDL_TEXT_H
