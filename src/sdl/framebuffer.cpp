#include "framebuffer.h"
#include "logger.h"

namespace sdl {

	FrameBuffer::~FrameBuffer() {
		if (frameBuffer_ != 0) {
			glDeleteFramebuffers(1, &frameBuffer_);
		}
	}
	
	FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept
		: frameBuffer_{std::exchange(other.frameBuffer_, 0)} {

	}

	FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept {
		if (frameBuffer_ != 0) {
			glDeleteFramebuffers(1, &frameBuffer_);
		}
		frameBuffer_ = std::exchange(other.frameBuffer_, 0);
		return *this;
	}

	void FrameBuffer::bind() const {
		if (frameBuffer_ != 0) {
			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer_);
			assertGlError();
		} else {
			logger()->debug("[FrameBuffer] Must be generated first");
		}
	}

	void FrameBuffer::unbind() const {
		bindDefault();
	}

	void FrameBuffer::bindDefault() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		assertGlError();
	}

	void FrameBuffer::generate() {
		if (frameBuffer_ == 0) {
			glGenFramebuffers(1, &frameBuffer_);
			assertGlError();
		} else {
			logger()->warn("[FrameBuffer] tried to create, but texture already exists");
		}
	}

	bool FrameBuffer::isValid() const noexcept {
		return frameBuffer_ != 0;
	}

	bool FrameBuffer::operator==(const FrameBuffer& texture) const noexcept {
		return frameBuffer_ == texture.frameBuffer_;
	}

	bool FrameBuffer::operator!=(const FrameBuffer& texture) const noexcept {
		return frameBuffer_ != texture.frameBuffer_;
	}

} // Namespace sdl.
