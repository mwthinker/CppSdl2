#ifndef CPPSDL2_SDL_TEXTURE_H
#define CPPSDL2_SDL_TEXTURE_H

#include "opengl.h"

#include <SDL_image.h>

#include <string>
#include <memory>
#include <functional>
#include <algorithm>
#include <utility>

namespace sdl {

	namespace helper {

		// Assumes that the surface is in RGBA.
		void flipVertical(SDL_Surface* surface);

		SDL_Surface* createSurface(int w, int h);

	}

	class Texture {
	public:
		friend class TextureAtlas;
		friend class Text;

		// Empty texture. Does nothing.
		Texture();

		// Loads a image from a file. It stores the image in memory and no opengl
		// code are of use in the constructor (safe to call constructor in other threads).
		explicit Texture(const std::string& filename, const std::function<void()>& filter = []() {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		});		

		// Create a empty texture.
		explicit Texture(int width, int height, const std::function<void()>& filter = []() {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		});

		Texture(const Texture& texture) = default;
		Texture& operator=(const Texture& texture) = default;

		Texture(Texture&& texture) noexcept;
		Texture& operator=(Texture&& texture) noexcept;

		bool operator==(const Texture& texture) const;

		bool operator!=(const Texture& texture) const;

		// Binds the texture to the target. First call, copies 
		// the image data to graphic memory.
		void bindTexture() const;

		std::pair<int, int> getSize() const {
			return {width_, height_};
		}

		// Return the width of the image in pixels.
		int getWidth() const {
			return width_;
		}

		// Return the height of the image in pixels. 
		int getHeight() const {
			return height_;
		}

		// Return true if the image is loaded correctly. It may however not been loaded
		// to graphic memory despite the result of this function.
		bool isValid() const {
			return imageData_ != nullptr;
		}

		// Use with care may be null.
		GLuint getGlTexture() const {
			return imageData_->texture_;
		}

	private:
		//void loadTexture()

		// The texture object takes over the ownership of the surface and is responsible of deallocation.
		// Not safe to use surface outside this class after calling the constuctor.
		explicit Texture(SDL_Surface* surface, const std::function<void()>& filter = []() {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		});

		class ImageData {
		public:
			ImageData(std::function<void()> filter);

			ImageData(SDL_Surface* surface, std::function<void()> filter);

			~ImageData();

			// Is not copyable.
			ImageData(const ImageData&) = delete;

			// Is not copyable.
			ImageData& operator=(const ImageData&) = delete;

			void loadImageToGraphic() const;

			// Is mutable in order for the bind() function to be const.
			// It's ok beacause the variables can be seen as cache variables.
			// So the external "constness" is preserved.
			mutable SDL_Surface* preLoadSurface_;
			mutable GLuint texture_;
			std::function<void()> filter_;
			int windowInstance_;
		};
		
		std::shared_ptr<ImageData> imageData_;
		mutable bool firstCallBind_;
		int width_, height_;
	};

	inline bool Texture::operator==(const Texture& texture) const {
		return imageData_ == texture.imageData_;
	}

	inline bool Texture::operator!=(const Texture& other) const {
		return !(*this == other);
	}

} // Namespace sdl.

#endif // CPPSDL2_SDL_TEXTURE_H
