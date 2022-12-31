#include "texture.h"
#include "surface.h"
#include "opengl.h"

#include <spdlog/spdlog.h>

namespace sdl {

	Texture::~Texture() {
		if (texture_ != 0) {
			gl::glDeleteTextures(1, &texture_);
		}
	}

	Texture::Texture(Texture&& texture) noexcept
		: texture_{std::exchange(texture.texture_, 0)} {

	}

	Texture& Texture::operator=(Texture&& texture) noexcept {
		if (texture_ != 0) {
			gl::glDeleteTextures(1, &texture_);
		}
		texture_ = std::exchange(texture.texture_, 0);
		return *this;
	}

	void Texture::bind() {
		if (texture_ != 0) {
			gl::glBindTexture(gl::GL_TEXTURE_2D, texture_);
		} else {
			spdlog::debug("[sdl::Texture] Must be generated first");
		}
	}

	void Texture::texSubImage(const Surface& surface, const Rect& dst) {
		if (isValid() && surface.isLoaded()) {
			gl::glBindTexture(gl::GL_TEXTURE_2D, texture_);
			glTexSubImage2D(gl::GL_TEXTURE_2D, 0,
				dst.x, dst.y,
				dst.w, dst.h,
				surfaceFormat(surface.surface_),
				gl::GL_UNSIGNED_BYTE,
				surface.surface_->pixels);
		} else {
			spdlog::warn("[sdl::Texture] texSubImage failed");
		}
	}

	void Texture::generate() {
		if (texture_ == 0) {
			gl::glGenTextures(1, &texture_);
		} else {
			spdlog::warn("[sdl::Texture] tried to create, but texture already exists");
		}
	}

	bool Texture::isValid() const noexcept {
		return texture_ != 0;
	}

	gl::GLenum Texture::surfaceFormat(SDL_Surface* surface) {
		switch (surface->format->BytesPerPixel) {
			case 1:
				return gl::GL_RED;
			case 3:
				return gl::GL_RGB;
			case 4:
				return gl::GL_RGBA;
		}
		return gl::GL_RED;
	}

}
