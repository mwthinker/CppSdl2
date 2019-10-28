#ifndef CPPSDL2_SDL_SPRITE_H
#define CPPSDL2_SDL_SPRITE_H

#include "texture.h"
#include "surface.h"
#include "rect.h"

#include <string>
#include <functional>
#include <variant>

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

		Sprite(const Sprite& sprite) = default;
		Sprite& operator=(const Sprite& sprite) = default;

		Sprite(Sprite&& sprite) noexcept;
		Sprite& operator=(Sprite&& sprite) noexcept;

		Sprite(const Sprite& sprite, const Rect& rect);

		void bindTexture() const;

		// Return the lower left x position of the image drawn.
		float getX() const noexcept;

		// Return the lower left y position of the image drawn.
		float getY() const noexcept;

		std::pair<float, float> getPosition() const noexcept;

		float getWidth() const noexcept;

		float getHeight() const noexcept;

		std::pair<float, float> getSize() const noexcept;

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

		Rect rect_ {};
		int textureWidth_ = 0, textureHeight_ = 0;
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_SPRITE_H
