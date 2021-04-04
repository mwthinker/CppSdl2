#ifndef CPPSDL2_SDL_TEXTURE_H
#define CPPSDL2_SDL_TEXTURE_H

#include "opengl.h"
#include "rect.h"
#include "surface.h"

#include <spdlog/spdlog.h>

#include <type_traits>

namespace sdl {

	class Surface;

	class Texture {
	public:
		friend class TextureAtlas;

		Texture() = default;
		~Texture();

		Texture(const Texture& texture) = delete;
		Texture& operator=(const Texture& texture) = delete;

		Texture(Texture&& texture) noexcept;
		Texture& operator=(Texture&& texture) noexcept;
		
		void bind();
		
		void texImage(const Surface& surface);

		template <typename FilterFunc>
		void texImage(const Surface& surface, FilterFunc&& filter);

		void texSubImage(const Surface& surface, const Rect& dst);

		void generate();
		
		bool isValid() const noexcept;

		friend bool operator==(const Texture& left, const Texture& right) noexcept;
		
		friend bool operator!=(const Texture& left, const Texture& right) noexcept;

		operator GLuint() const noexcept {
			return texture_;
		}

	private:
		static GLenum surfaceFormat(SDL_Surface* surface);

		GLuint texture_{};
	};

	inline bool operator==(const Texture& left, const Texture& right) noexcept {
		return left.texture_ == right.texture_;
	}

	inline bool operator!=(const Texture& left, const Texture& right) noexcept {
		return left.texture_ != right.texture_;
	}

	inline void Texture::texImage(const Surface& surface) {
		texImage(surface, []() {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		});
	}

	template <typename FilterFunc>
	void Texture::texImage(const Surface& surface, FilterFunc&& filter) {
		static_assert(std::is_invocable_r_v<void, FilterFunc>, "Must be invokable, in the form void()");
		
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

}

#endif
