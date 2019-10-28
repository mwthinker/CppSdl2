#ifndef CPPSDL2_SDL_SURFACE_H
#define CPPSDL2_SDL_SURFACE_H

#include "color.h"
#include "rect.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>
#include <tuple>

namespace sdl {
		
	class Surface;
	void flipVertical(Surface& surface);

	class Font;

	class Surface {
	public:
		Surface() = default;
		~Surface();

		explicit Surface(const std::string& filename);

		Surface(int w, int h);

		Surface(int w, int h, const Color& color);

		Surface(const std::string& text, const Font& font, const Color& color);

		Surface(const Surface& surface) = delete;
		Surface& operator=(const Surface& surface) = delete;

		Surface(Surface&& texture) noexcept;
		Surface& operator=(Surface&& texture) noexcept;

		bool isLoaded() const noexcept;

		std::pair<int, int> getSize() const noexcept;

		int getWidth() const noexcept;

		int getHeight() const noexcept;

		void blitSurface(const Surface& src, const Rect& rect);

	private:
		friend class Texture;
		friend class TextureAtlas;
		friend void flipVertical(Surface& surface);

		SDL_Surface* surface_ = nullptr;
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_SURFACE_H
