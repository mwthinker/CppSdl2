#include "vertexbufferobject.h"

#include <spdlog/spdlog.h>

namespace sdl {

	namespace {

		constexpr GLenum isValidBindTarget(GLenum target) {
			return GL_ARRAY_BUFFER == target || GL_ELEMENT_ARRAY_BUFFER == target;
		}

		constexpr GLenum isValidDataBufferUsage(GLenum usage) {
			return GL_STATIC_DRAW == usage || GL_DYNAMIC_DRAW == usage || GL_STREAM_DRAW == usage;
		}

	}

	VertexBufferObject::~VertexBufferObject() {
		if (vboId_ != 0) {
			glDeleteBuffers(1, &vboId_);
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
			glDeleteBuffers(1, &vboId_);
			assertGlError();
		}
		vboId_ = std::exchange(other.vboId_, 0);
		size_ = other.size_;
		target_ = other.target_;
		return *this;
	}

	void VertexBufferObject::bufferData(GLsizeiptr size, const GLvoid* data, GLenum usage) {
		assert(isValidDataBufferUsage(usage));
		assert(vboId_ != 0);

        size_ = size;
		usage_ = usage;

        glBufferData(target_, size, data, usage);
		assertGlError();
	}

	void VertexBufferObject::bufferSubData(GLsizeiptr offset, GLsizeiptr size, const GLvoid* data) {
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
            glGenBuffers(1, &vboId_);
			assertGlError();
		} else {
			spdlog::warn("[sdl::VertexBufferObject] Calling generate failed, generate has already been called");
		}
	}

	void VertexBufferObject::bind(GLenum target) {
		assert(isValidBindTarget(target));

		if (vboId_ != 0) {
			target_ = target;
			glBindBuffer(target_, vboId_);
			assertGlError();
		} else {
			spdlog::warn("[sdl::VertexBufferObject] bind failed, generate must be called first");
		}
	}

	void VertexBufferObject::unbind() const {
		glBindBuffer(target_, 0);
		assertGlError();
	}

} // Namespace sdl.
