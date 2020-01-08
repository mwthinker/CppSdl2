#ifndef CPPSDL2_SDL_TEXTUREVIEW_H
#define CPPSDL2_SDL_TEXTUREVIEW_H

#include "texture.h"

namespace sdl {

	class TextureView {
	public:
		TextureView() noexcept = default;

		explicit TextureView(const Texture& texture) noexcept;

		TextureView(const Texture& texture, float x, float y, float dx, float dy) noexcept;

		TextureView(const TextureView& view, float x, float y, float dx, float dy) noexcept;

		TextureView(const TextureView& texture) noexcept = default;
		
		TextureView& operator=(const TextureView& texture) noexcept = default;

		float getX() const noexcept {
			return x_;
		}

		float getY() const noexcept {
			return y_;
		}

		float getWidth() const noexcept {
			return dx_;
		}

		float getHeight() const noexcept {
			return dy_;
		}

		void bind() const;

		operator GLuint() const noexcept {
			return texture_;
		}

	private:
		float x_{0};
		float y_{0};
		float dx_{1};
		float dy_{1};
		GLuint texture_{};
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_TEXTUREVIEW_H
