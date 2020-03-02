#ifndef CPPSDL2_SDL_FRAMEBUFFER_H
#define CPPSDL2_SDL_FRAMEBUFFER_H

#include "opengl.h"

namespace sdl {

	class FrameBuffer {
	public:
		friend class TextureAtlas;
		friend class Text;

		FrameBuffer() = default;
		~FrameBuffer();

		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer& operator=(const FrameBuffer&) = delete;

		FrameBuffer(FrameBuffer&& other) noexcept;
		FrameBuffer& operator=(FrameBuffer&& other) noexcept;
		
		void bind() const;

		void unbind() const;

		static void bindDefault();

		void generate();
		
		bool isValid() const noexcept;

		bool operator==(const FrameBuffer& other) const noexcept;
		
		bool operator!=(const FrameBuffer& other) const noexcept;

		operator GLuint() const noexcept {
			return frameBuffer_;
		}

	private:
		GLuint frameBuffer_{};
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_FRAMEBUFFER_H
