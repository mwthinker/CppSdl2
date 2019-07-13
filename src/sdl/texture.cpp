#include "texture.h"
#include "opengl.h"
#include "window.h"
#include "logger.h"

#include <SDL_image.h>

#include <algorithm>
#include <tuple>
#include <cassert>

namespace sdl {

	namespace helper {

		namespace {

			using Pixel1 = std::tuple<Uint8>;
			using Pixel2 = std::tuple<Uint8, Uint8>;
			using Pixel3 = std::tuple<Uint8, Uint8, Uint8>;
			using Pixel4 = std::tuple<Uint8, Uint8, Uint8, Uint8>;

			template <typename Pixel>
			void flipVerticalGeneric(SDL_Surface* surface) {
				for (size_t i = 0; i < surface->h / 2; ++i) {
					for (size_t j = 0; j < surface->w; ++j) {
						auto startElement = static_cast<Pixel*>(surface->pixels) + i * surface->w + j;
						auto endElement = static_cast<Pixel*>(surface->pixels) + (surface->h - i - 1) * surface->w + j;
						std::swap(*startElement, *endElement);
					}
				}
			}

		} // Namespace anonymous.

		void flipVertical(SDL_Surface* surface) {
			assert(surface->format->BytesPerPixel >= 1 && surface->format->BytesPerPixel <= 4);
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
			Uint32 rmask = 0xff000000;
			Uint32 gmask = 0x00ff0000;
			Uint32 bmask = 0x0000ff00;
			Uint32 amask = 0x000000ff;
#else
			Uint32 rmask = 0x000000ff;
			Uint32 gmask = 0x0000ff00;
			Uint32 bmask = 0x00ff0000;
			Uint32 amask = 0xff000000;
#endif

			return SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
		}

		SDL_Surface* createSurface(int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
			SDL_Surface* s = createSurface(w, h);
			SDL_FillRect(s, 0, SDL_MapRGB(s->format, r, g, b));
			return s;
		}

	} // Namespace helper.

	namespace {

		// Return an opengl texture from the surface provided. Return 0 if the
		// operation failed.
		GLuint sdlGlLoadTexture(SDL_Surface* surface) {
			GLuint textureId = 0;
			glGenTextures(1, &textureId);
			glBindTexture(GL_TEXTURE_2D, textureId);
			int mode = GL_RGB;
			if (surface->format->BytesPerPixel == 4) {
				mode = GL_RGBA;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
			return textureId;
		}
	}

	Texture::Texture() : 
		firstCallBind_(true),
		width_(0), height_(0) {
	}

	Texture::Texture(const std::string& filename, const std::function<void()>& filter) :
		Texture(IMG_Load(filename.c_str())) {
	}

	Texture::Texture(int width, int height, const std::function<void()>& filter) :
		Texture(helper::createSurface(width, height)) {
	}

	Texture::Texture(int width, int height, uint8_t r, uint8_t g, uint8_t b, const std::function<void()>& filter) :
		Texture(width, width, r, g, b, 255, filter) {
	}

	Texture::Texture(int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a, const std::function<void()>& filter) :
		Texture(helper::createSurface(width, height, r, g, b, a)) {
	}

	Texture::Texture(Texture&& texture) noexcept :
		imageData_(std::move(texture.imageData_)),
		firstCallBind_(texture.firstCallBind_),
		width_(texture.width_), height_(texture.height_) {
		
		texture.width_ = 0;
		texture.height_ = 0;
		texture.firstCallBind_ = true;
	}

	Texture& Texture::operator=(Texture&& texture) noexcept {
		imageData_ = std::move(texture.imageData_);
		firstCallBind_ = texture.firstCallBind_;
		width_ = texture.width_;
		height_ = texture.height_;

		texture.width_ = 0;
		texture.height_ = 0;
		texture.firstCallBind_ = true;
		return *this;
	}

	void Texture::bindTexture() const {
		if (imageData_ != nullptr) {
			if (firstCallBind_) {
				firstCallBind_ = false;
				if (imageData_->preLoadSurface_ != nullptr) {
					imageData_->loadImageToGraphic();
				} else {
					glBindTexture(GL_TEXTURE_2D, imageData_->texture_);
					checkGlError();
				}
			} else {
				if (imageData_->texture_ != 0) {
					glBindTexture(GL_TEXTURE_2D, imageData_->texture_);
					checkGlError();
				}
			}
		}
	}

	Texture::Texture(SDL_Surface* surface, const std::function<void()>& filter) :
		firstCallBind_(true),
		width_(0), height_(0) {

		if (surface != 0) {
			width_ = surface->w;
			height_ = surface->h;
			imageData_ = std::make_shared<ImageData>(surface, filter);
		}
		else {
			logger()->warn("[Texture] Empty image failed to be created: {}", IMG_GetError());
		}
	}
	
	Texture::ImageData::ImageData(std::function<void()> filter) :
		preLoadSurface_(0), texture_(0), filter_(filter) {
	}

	Texture::ImageData::ImageData(SDL_Surface* surface, std::function<void()> filter) :
		preLoadSurface_(surface), texture_(0), filter_(filter) {
	}

	void Texture::ImageData::loadImageToGraphic() const {
		// Current surface is valid?
		if (preLoadSurface_ != 0) {
			helper::flipVertical(preLoadSurface_); // SDL and opengl uses different orientations.
			texture_ = sdlGlLoadTexture(preLoadSurface_); // Load to texture.
			SDL_FreeSurface(preLoadSurface_);
			preLoadSurface_ = nullptr;

			// Texture valid?
			if (texture_ != 0) {
				filter_();
			}
		}
	}

	Texture::ImageData::~ImageData() {
		// Opengl texture? And the opengl context active?
		if (texture_ != 0) {
			// Is called if the opengl texture is valid and therefore need to be cleaned up.
			glDeleteTextures(1, &texture_);
		}

		// Safe to pass null.
		SDL_FreeSurface(preLoadSurface_);
	}

} // Namespace sdl.
