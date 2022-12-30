#include "vertexbufferobject.h"

#include <spdlog/spdlog.h>

namespace sdl {

	namespace {

		constexpr bool isValidBindTarget(gl::GLenum target) {
			return gl::GL_ARRAY_BUFFER == target || gl::GL_ELEMENT_ARRAY_BUFFER == target;
		}

		constexpr bool isValidDataBufferUsage(gl::GLenum usage) {
			return gl::GL_STATIC_DRAW == usage || gl::GL_DYNAMIC_DRAW == usage || gl::GL_STREAM_DRAW == usage || gl::GL_STREAM_READ == usage;
		}

	}

	VertexBufferObject::~VertexBufferObject() {
		if (vboId_ != 0) {
			gl::glDeleteBuffers(1, &vboId_);
			assertGlError();
		}
	}

	VertexBufferObject::VertexBufferObject(VertexBufferObject&& other) noexcept
		: vboId_{std::exchange(other.vboId_, 0)}
		, size_{other.size_}
		, target_{other.target_}
		, usage_{other.usage_} {
	}

	VertexBufferObject& VertexBufferObject::operator=(VertexBufferObject&& other) noexcept {
		if (vboId_ != 0) {
			gl::glDeleteBuffers(1, &vboId_);
			assertGlError();
		}
		vboId_ = std::exchange(other.vboId_, 0);
		size_ = other.size_;
		target_ = other.target_;
		return *this;
	}

	void VertexBufferObject::bufferData(gl::GLsizeiptr size, const gl::GLvoid* data, gl::GLenum usage) {
		assert(isValidDataBufferUsage(usage));
		assert(vboId_ != 0);

        size_ = size;
		usage_ = usage;

        glBufferData(target_, size, data, usage);
		assertGlError();
	}

	void VertexBufferObject::bufferSubData(gl::GLsizeiptr offset, gl::GLsizeiptr size, const gl::GLvoid* data) {
		if (vboId_ != 0 && target_ != 0) {
			if (size_ > size) {
				glBufferSubData(target_, offset, size, data);
			} else {
				bufferData(size, data, usage_);
			}
			assertGlError();
		} else {
			spdlog::warn("[sdl::VertexBufferObject] Calling bindSubData failed, must call bindData first");
		}
	}

	bool VertexBufferObject::isGenerated() const noexcept {
		return vboId_ != 0;
	}

    void VertexBufferObject::generate() {
        if (vboId_ == 0) {
			gl::glGenBuffers(1, &vboId_);
			assertGlError();
		} else {
			spdlog::warn("[sdl::VertexBufferObject] Calling generate failed, generate has already been called");
		}
	}

	void VertexBufferObject::bind(gl::GLenum target) {
		assert(isValidBindTarget(target));

		if (vboId_ != 0) {
			target_ = target;
			gl::glBindBuffer(target_, vboId_);
			assertGlError();
		} else {
			spdlog::warn("[sdl::VertexBufferObject] bind failed, generate must be called first");
		}
	}

	void VertexBufferObject::unbind() {
		gl::glBindBuffer(target_, 0);
		assertGlError();
	}

}
