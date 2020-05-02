#include "texture.h"
#include "surface.h"
#include "opengl.h"

#include <spdlog/spdlog.h>

namespace sdl {

	namespace {

		GLenum surfaceFormat(SDL_Surface* surface) {
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

	void Texture::texImage(const Surface& surface, std::function<void()>&& filter) {
		if (!surface.isLoaded()) {
			spdlog::debug("[sdl::Texture] Failed to bind, must be loaded first");
			return;
		}

		if (!isValid()) {
			spdlog::debug("[sdl::Texture] Failed to bind, must be generated first");
			return;
		}

		auto format = surfaceFormat(surface.surface_);
		if (format == 0) {
			spdlog::debug("[sdl::Texture] Failed BytesPerPixel, must be 1, 3 or 4. Is: {}", surface.surface_->format->BytesPerPixel);
			return;
		}

		glBindTexture(GL_TEXTURE_2D, texture_);
		assertGlError();
		filter();
		glTexImage2D(GL_TEXTURE_2D, 0, format,
			surface.surface_->w, surface.surface_->h,
			0,
			format,
			GL_UNSIGNED_BYTE,
			surface.surface_->pixels
		);
		assertGlError();
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

	bool Texture::operator==(const Texture& texture) const noexcept {
		return texture_ == texture.texture_;
	}

	bool Texture::operator!=(const Texture& texture) const noexcept {
		return texture_ != texture.texture_;
	}

} // Namespace sdl.
