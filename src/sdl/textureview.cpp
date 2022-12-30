#include "textureview.h"

namespace sdl {
	
	TextureView::TextureView(const Texture& texture) noexcept
		: texture_{texture} {
	}
	
	TextureView::TextureView(const Texture& texture, float x, float y, float dx, float dy) noexcept
		: pos_{x, y}
		, size_{dx, dy}
		, texture_{texture}{
	}
	
	TextureView::TextureView(const TextureView& view, float x, float y, float dx, float dy) noexcept
		: pos_{x, y}
		, size_{dx, dy}
		, texture_{view.texture_} {
	}

	TextureView::TextureView(const Texture& texture, const glm::vec2& pos, const glm::vec2& size) noexcept
		: pos_{pos}
		, size_{size}
		, texture_{texture} {
	}

	TextureView::TextureView(const TextureView& view, const glm::vec2& pos, const glm::vec2& size) noexcept
		: pos_{pos}
		, size_{size}
		, texture_{view.texture_} {
	}

	void TextureView::bind() {
		gl::glBindTexture(gl::GL_TEXTURE_2D, texture_);
	}

}
