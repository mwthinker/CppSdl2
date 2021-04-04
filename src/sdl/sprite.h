#ifndef CPPSDL2_SDL_SPRITE_H
#define CPPSDL2_SDL_SPRITE_H

#include "texture.h"
#include "surface.h"
#include "textureview.h"
#include "rect.h"
#include "font.h"

#include <string>
#include <functional>
#include <variant>

#include <glm/vec2.hpp>

namespace sdl {

	class Sprite {
	public:
		// Create an empty sprite.
		Sprite() = default;

		// Load an image from file.
		explicit Sprite(const std::string& image, std::function<void()>&& filter = []() {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		});

		Sprite(const std::string& image, const Font& font, std::function<void()>&& filter = []() {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		});

		Sprite(Surface&& surface, std::function<void()>&& filter = []() {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		});

		// Set a texture to the sprite. The sprite represents the square of the texture,
		// defined by (x,y) in the lower left postion with (dx,dy) = (width,height).
		Sprite(Surface&& surface, const Rect& rect, std::function<void()>&& filter = []() {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		});

		Sprite(const Sprite& other) = default;
		Sprite& operator=(const Sprite& other) = default;

		Sprite(Sprite&& other) noexcept;
		Sprite& operator=(Sprite&& other) noexcept;

		Sprite(const Sprite& sprite, const Rect& rect);

		void bindTexture();

		TextureView getTextureView() const;

		// Return the lower left x position of the image drawn.
		float getX() const noexcept;

		// Return the lower left y position of the image drawn.
		float getY() const noexcept;

		glm::vec2 getPosition() const noexcept;

		float getWidth() const noexcept;

		float getHeight() const noexcept;

		glm::vec2 getSize() const noexcept;

		int getTextureWidth() const noexcept;

		int getTextureHeight() const noexcept;

		std::pair<int, int> getTextureSize() const noexcept;

		bool isValid() const noexcept;

		static bool equalSource(const Sprite& s1, const Sprite& s2) noexcept;

	private:
		void blit(const Surface& src, const Rect& rect);

		friend class TextureAtlas;

		struct SurfaceData {
			Surface surface;
			std::function<void()> filter;
		};

		using ImageVariant = std::variant<SurfaceData, Texture>;
		mutable std::shared_ptr<ImageVariant> image_;

		Rect rect_{};
		int textureWidth_{};
		int textureHeight_{};
	};

}

#endif
