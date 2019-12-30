#include "textureview.h"

namespace sdl {
	
	TextureView::TextureView(const Texture& texture) noexcept :
		texture_{texture.getGlTexture()} {
	}
	
	TextureView::TextureView(const Texture& texture, float x, float y, float dx, float dy) noexcept :
		texture_{texture.getGlTexture()}, x_{x}, y_{y}, dx_{dx}, dy_{dy} {
	}	

	void TextureView::bind() const {
		glBindTexture(GL_TEXTURE_2D, texture_);
	}

} // Namespace sdl.
