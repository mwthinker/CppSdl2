#include "texture.h"
#include "surface.h"
#include "opengl.h"
#include "logger.h"

namespace sdl {

	namespace {

		GLenum surfaceFormat(SDL_Surface* surface) {
			assert(surface->format->BytesPerPixel == 4 || surface->format->BytesPerPixel == 3);
			return surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
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
		texture_ = std::exchange(texture.texture_, 0);
		return *this;
	}

	void Texture::bind() const {
		if (texture_ != 0) {
			glBindTexture(GL_TEXTURE_2D, texture_);
			assertGlError();
		} else {
			logger()->debug("[Texture] Must be generated first");
		}
	}

	void Texture::texImage(const Surface& surface, std::function<void()>&& filter) {
		if (!surface.isLoaded()) {
			logger()->debug("[Texture] Failed to bind, must be loaded first");
			return;
		}

		if (!isValid()) {
			logger()->debug("[Texture] Failed to bind, must be generated first");
			return;
		}
		
		glBindTexture(GL_TEXTURE_2D, texture_);
		assertGlError();
		filter();
		glTexImage2D(GL_TEXTURE_2D, 0, surfaceFormat(surface.surface_),
			surface.surface_->w, surface.surface_->h,
			0,
			surfaceFormat(surface.surface_),
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
			logger()->warn("[Texture] texSubImage failed");
		}
	}

	void Texture::generate() {
		if (texture_ == 0) {
			glGenTextures(1, &texture_);
			assertGlError();
		} else {
			logger()->warn("[Texture] tried to create, but texture already exists");
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
