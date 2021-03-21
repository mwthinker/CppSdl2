#include "texture.h"
#include "surface.h"
#include "opengl.h"

#include <spdlog/spdlog.h>

namespace sdl {

	Texture::~Texture() {
		if (texture_ != 0) {
			glDeleteTextures(1, &texture_);
		}
	}

	Texture::Texture(Texture&& texture) noexcept
		: texture_{std::exchange(texture.texture_, 0)} {

	}

	Texture& Texture::operator=(Texture&& texture) noexcept {
		if (texture_ != 0) {
			glDeleteTextures(1, &texture_);
		}
		texture_ = std::exchange(texture.texture_, 0);
		return *this;
	}

	void Texture::bind() const {
		if (texture_ != 0) {
			glBindTexture(GL_TEXTURE_2D, texture_);
			assertGlError();
		} else {
			spdlog::debug("[sdl::Texture] Must be generated first");
		}
	}

	void Texture::texSubImage(const Surface& surface, const Rect& dst) {
		if (isValid() && surface.isLoaded()) {
			glBindTexture(GL_TEXTURE_2D, texture_);
			glTexSubImage2D(GL_TEXTURE_2D, 0,
				dst.x, dst.y,
				dst.w, dst.h,
				surfaceFormat(surface.surface_),
				GL_UNSIGNED_BYTE,
				surface.surface_->pixels);
			assertGlError();
		} else {
			spdlog::warn("[sdl::Texture] texSubImage failed");
		}
	}

	void Texture::generate() {
		if (texture_ == 0) {
			glGenTextures(1, &texture_);
			assertGlError();
		} else {
			spdlog::warn("[sdl::Texture] tried to create, but texture already exists");
		}
	}

	bool Texture::isValid() const noexcept {
		return texture_ != 0;
	}

	bool operator==(const Texture& left, const Texture& right) noexcept {
		return left.texture_ == right.texture_;
	}

	bool operator!=(const Texture& left, const Texture& right) noexcept {
		return left.texture_ != right.texture_;
	}

	GLenum Texture::surfaceFormat(SDL_Surface* surface) {
		switch (surface->format->BytesPerPixel) {
			case 1:
				return GL_RED;
			case 3:
				return GL_RGB;
			case 4:
				return GL_RGBA;
		}
		return 0;
	}

}
