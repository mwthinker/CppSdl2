#include "framebuffer.h"

#include <spdlog/spdlog.h>

namespace sdl {

	FrameBuffer::~FrameBuffer() {
		if (frameBuffer_ != 0) {
			gl::glDeleteFramebuffers(1, &frameBuffer_);
		}
	}
	
	FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept
		: frameBuffer_{std::exchange(other.frameBuffer_, 0)} {

	}

	FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept {
		if (frameBuffer_ != 0) {
			gl::glDeleteFramebuffers(1, &frameBuffer_);
		}
		frameBuffer_ = std::exchange(other.frameBuffer_, 0);
		return *this;
	}

	void FrameBuffer::bind() {
		if (frameBuffer_ != 0) {
			gl::glBindFramebuffer(gl::GL_FRAMEBUFFER, frameBuffer_);
			assertGlError();
		} else {
			spdlog::debug("[sdl::FrameBuffer] Must be generated first");
		}
	}

	void FrameBuffer::unbind() {
		bindDefault();
	}

	void FrameBuffer::bindDefault() {
		gl::glBindFramebuffer(gl::GL_FRAMEBUFFER, 0);
		assertGlError();
	}

	void FrameBuffer::generate() {
		if (frameBuffer_ == 0) {
			gl::glGenFramebuffers(1, &frameBuffer_);
			assertGlError();
		} else {
			spdlog::warn("[sdl::FrameBuffer] tried to create, but texture already exists");
		}
	}

	bool FrameBuffer::isValid() const noexcept {
		return frameBuffer_ != 0;
	}

}
