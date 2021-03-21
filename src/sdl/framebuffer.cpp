#include "framebuffer.h"

#include <spdlog/spdlog.h>

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
			spdlog::debug("[sdl::FrameBuffer] Must be generated first");
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
			spdlog::warn("[sdl::FrameBuffer] tried to create, but texture already exists");
		}
	}

	bool FrameBuffer::isValid() const noexcept {
		return frameBuffer_ != 0;
	}

	bool operator==(const FrameBuffer& left, const FrameBuffer& right) noexcept {
		return left.frameBuffer_ == right.frameBuffer_;
	}

	bool operator!=(const FrameBuffer& left, const FrameBuffer& right) noexcept {
		return left.frameBuffer_ != right.frameBuffer_;
	}

}
