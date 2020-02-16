#ifndef CPPSDL2_SDL_TEXTURE_H
#define CPPSDL2_SDL_TEXTURE_H

#include "opengl.h"
#include "rect.h"

#include <string>
#include <memory>
#include <functional>

namespace sdl {

	class Surface;

	class Texture {
	public:
		friend class TextureAtlas;
		friend class Text;

		Texture() = default;
		~Texture();

		Texture(const Texture& texture) = delete;
		Texture& operator=(const Texture& texture) = delete;

		Texture(Texture&& texture) noexcept;
		Texture& operator=(Texture&& texture) noexcept;
		
		void bind() const;

		void texImage(const Surface& surface, std::function<void()>&& filter = []() {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		});

		void texSubImage(const Surface& surface, const Rect& dst);

		void generate();
		
		bool isValid() const noexcept;

		bool operator==(const Texture& texture) const noexcept;
		
		bool operator!=(const Texture& texture) const noexcept;

		operator GLuint() const noexcept {
			return texture_;
		}

	private:
		GLuint texture_{};
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_TEXTURE_H
