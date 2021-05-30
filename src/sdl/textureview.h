#ifndef CPPSDL2_SDL_TEXTUREVIEW_H
#define CPPSDL2_SDL_TEXTUREVIEW_H

#include "texture.h"

#include <glm/vec2.hpp>

namespace sdl {

	class TextureView {
	public:
		constexpr TextureView() = default;

		explicit TextureView(const Texture& texture) noexcept;

		TextureView(const Texture& texture, float x, float y, float dx, float dy) noexcept;

		TextureView(const TextureView& view, float x, float y, float dx, float dy) noexcept;

		TextureView(const Texture& texture, const glm::vec2& pos, const glm::vec2& size) noexcept;

		TextureView(const TextureView& view, const glm::vec2& pos, const glm::vec2& size) noexcept;

		TextureView(const TextureView& texture) noexcept = default;
		
		TextureView& operator=(const TextureView& texture) noexcept = default;

		float getX() const noexcept {
			return pos_.x;
		}

		float getY() const noexcept {
			return pos_.y;
		}

		glm::vec2 getPosition() const noexcept {
			return pos_;
		}

		float getWidth() const noexcept {
			return size_.x;
		}

		float getHeight() const noexcept {
			return size_.y;
		}

		glm::vec2 getSize() const noexcept {
			return size_;
		}

		void bind();

		constexpr operator GLuint() const noexcept {
			return texture_;
		}

	private:
		glm::vec2 pos_{0.f, 0.f};
		glm::vec2 size_{1.f, 1.f};
		GLuint texture_ = 0;
	};

}

#endif
