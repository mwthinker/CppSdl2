#include "surface.h"
#include "font.h"

#include <spdlog/spdlog.h>

#include <cassert>

namespace sdl {

	namespace {

		using Pixel1 = std::tuple<Uint8>;
		using Pixel2 = std::tuple<Uint8, Uint8>;
		using Pixel3 = std::tuple<Uint8, Uint8, Uint8>;
		using Pixel4 = std::tuple<Uint8, Uint8, Uint8, Uint8>;

		template <typename Pixel>
		void flipVerticalGeneric(SDL_Surface* surface) {
			assert(surface != nullptr);
			for (size_t i = 0; i < surface->h / 2; ++i) {
				for (size_t j = 0; j < surface->w; ++j) {
					auto startElement = static_cast<Pixel*>(surface->pixels) + i * surface->w + j;
					auto endElement = static_cast<Pixel*>(surface->pixels) + (surface->h - i - 1) * surface->w + j;
					std::swap(*startElement, *endElement);
				}
			}
		}

		void flipVertical(SDL_Surface* surface) {
			assert(surface != nullptr && surface->format->BytesPerPixel >= 1 && surface->format->BytesPerPixel <= 4);
			if (surface->format->BytesPerPixel == 4) {
				flipVerticalGeneric<Pixel4>(surface);
			} else if (surface->format->BytesPerPixel == 3) {
				flipVerticalGeneric<Pixel3>(surface);
			} else if (surface->format->BytesPerPixel == 2) {
				flipVerticalGeneric<Pixel2>(surface);
			} else if (surface->format->BytesPerPixel == 1) {
				flipVerticalGeneric<Pixel1>(surface);
			}
		}

		SDL_Surface* createSurface(int w, int h) {
			// SDL interprets each pixel as a 32-bit number, so our masks must depend
			// on the endianness (byte order) of the machine.
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			Uint32 rmask = 0xff'00'00'00;
			Uint32 gmask = 0x00'ff'00'00;
			Uint32 bmask = 0x00'00'ff'00;
			Uint32 amask = 0x00'00'00'ff;
#else
			Uint32 rmask = 0x00'00'00'ff;
			Uint32 gmask = 0x00'00'ff'00;
			Uint32 bmask = 0x00'ff'00'00;
			Uint32 amask = 0xff'00'00'00;
#endif
			return SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
		}

		SDL_Surface* createSurface(int w, int h, const Color& color) {
			auto s = createSurface(w, h);
			SDL_Color sdlColor = color;
			SDL_FillRect(s, 0,
				SDL_MapRGBA(s->format, sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a)
			);
			return s;
		}

		SDL_Surface* createSurface(const std::string& text, TTF_Font* font, const Color& color) {
			if (font != nullptr) {
				return TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), color, 500);
			}
			return nullptr;
		}

	} // Namespace anonymous.

	void flipVertical(Surface& surface) {
		if (surface.surface_ != nullptr) {
			flipVertical(surface.surface_);
		}
	}

	Surface::Surface(int w, int h) {
		assert(w > 0 && h > 0);
		surface_ = createSurface(w, h);
	}

	Surface::Surface(const std::string& filename) {
		surface_ = IMG_Load(filename.c_str());
		if (surface_ == nullptr) {
			spdlog::warn("[sdl::Surface] Image {} failed to be loaded: {}", filename, IMG_GetError());
		}
	}

	Surface::Surface(int w, int h, const Color& color) {
		assert(w > 0 && h > 0);
		surface_ = createSurface(w, h, color);
	}

	Surface::Surface(const std::string& text, const Font& font, const Color& color) {
		surface_ = createSurface(text, font.font_, color);
	}

	Surface::~Surface() {
		// Safe to pass null.
		SDL_FreeSurface(surface_);
	}

	Surface::Surface(Surface&& other) noexcept
		: surface_{std::exchange(other.surface_, nullptr)} {
	}

	Surface& Surface::operator=(Surface&& other) noexcept {
		// Safe to pass null.
		SDL_FreeSurface(surface_);
		surface_ = std::exchange(other.surface_, nullptr);
		return *this;
	}

	bool Surface::isLoaded() const noexcept {
		return surface_ != nullptr;
	}

	std::pair<int, int> Surface::getSize() const noexcept {
		if (surface_ == nullptr) {
			return {0, 0};
		}
		return {surface_->w, surface_->h};
	}
	
	int Surface::getWidth() const noexcept {
		if (surface_ == nullptr) {
			return 0;
		}
		return surface_->w;
	}

	int Surface::getHeight() const noexcept {
		if (surface_ == nullptr) {
			return 0;
		}
		return surface_->h;
	}

	void Surface::blitSurface(const Surface& src, const Rect& rect) {
		auto newSurface = SDL_ConvertSurface(src.surface_, surface_->format, 0);
		if (newSurface == nullptr) {
			spdlog::warn("[sdl::Surface] Failed to blit surface, during convert: {}", SDL_GetError());
			return;
		}
		SDL_Rect sdlRect = rect;
		if (SDL_BlitSurface(newSurface, 0, surface_, &sdlRect) != 0) {
			spdlog::warn("[sdl::Surface] Failed to blit surface: {}", SDL_GetError());
		}
	}
	
} // Namespace sdl.
