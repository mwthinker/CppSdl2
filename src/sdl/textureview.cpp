#include "textureview.h"

namespace sdl {
	
	TextureView::TextureView(const Texture& texture) noexcept
		: texture_{texture} {
	}
	
	TextureView::TextureView(const Texture& texture, float x, float y, float dx, float dy) noexcept
		: texture_{texture}
		, pos_{x, y}
		, size_{dx, dy} {
	}
	
	TextureView::TextureView(const TextureView& view, float x, float y, float dx, float dy) noexcept
		: texture_{view.texture_}
		, pos_{x, y}
		, size_{dx, dy} {
	}

	TextureView::TextureView(const Texture& texture, const glm::vec2& pos, const glm::vec2& size) noexcept
		: texture_{texture}
		, pos_{pos}
		, size_{size} {
	}

	TextureView::TextureView(const TextureView& view, const glm::vec2& pos, const glm::vec2& size) noexcept
		: texture_{view.texture_}
		, pos_{pos}
		, size_{size} {
	}

	void TextureView::bind() const {
		glBindTexture(GL_TEXTURE_2D, texture_);
	}

} // Namespace sdl.
