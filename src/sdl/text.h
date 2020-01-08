#ifndef CPPSDL2_SDL_TEXT_H
#define CPPSDL2_SDL_TEXT_H

#include "font.h"
#include "sprite.h"

#include <string>

namespace sdl {

	class Text {
	public:
		// Create a empty text object.
		Text() = default;

		// Create a Text object with the text and font provided.
		Text(const std::string& text, const Font& font);

		Text(const Text&) = default;
		Text& operator=(const Text&) = default;

		Text(Text&& other) noexcept = default;
		Text& operator=(Text&& other) noexcept = default;

		bool operator==(const Text& text) const noexcept;

		bool operator!=(const Text& text) const noexcept;

		// Returns the width in "pixels".
		float getWidth() const noexcept;

		// Return the height in "pixels".
		float getHeight() const noexcept;

		void bindTexture() const;

        // Return if the underlying texture is valid.
        // I.e. the texture is in memory with width and height.
		bool isValid() const noexcept;

		const Sprite& getSprite() const noexcept;

	private:
		Sprite sprite_;
		int characterSize_{};
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_TEXT_H
