#ifndef CPPSDL2_SDL_FRAMEBUFFER_H
#define CPPSDL2_SDL_FRAMEBUFFER_H

#include "opengl.h"

namespace sdl {

	class FrameBuffer {
	public:
		FrameBuffer() = default;
		~FrameBuffer();

		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer& operator=(const FrameBuffer&) = delete;

		FrameBuffer(FrameBuffer&& other) noexcept;
		FrameBuffer& operator=(FrameBuffer&& other) noexcept;
		
		void bind();

		void unbind();

		static void bindDefault();

		void generate();
		
		bool isValid() const noexcept;

		friend bool operator==(const FrameBuffer& left, const FrameBuffer& right) noexcept;
		
		friend bool operator!=(const FrameBuffer& left, const FrameBuffer& right) noexcept;

		operator GLuint() const noexcept {
			return frameBuffer_;
		}

	private:
		GLuint frameBuffer_{};
	};

	inline bool operator==(const FrameBuffer& left, const FrameBuffer& right) noexcept {
		return left.frameBuffer_ == right.frameBuffer_;
	}

	inline bool operator!=(const FrameBuffer& left, const FrameBuffer& right) noexcept {
		return left.frameBuffer_ != right.frameBuffer_;
	}

}

#endif
