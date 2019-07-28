#include "vertexbufferobject.h"
#include "logger.h"

namespace sdl {

	VertexBufferObject::VertexBufferObject() noexcept
		: vboId_(0), size_(0), target_(0) {

	}

	VertexBufferObject::~VertexBufferObject() {
		if (vboId_ != 0) {
			glDeleteBuffers(1, &vboId_);
			checkGlError();
		}
	}

	VertexBufferObject::VertexBufferObject(VertexBufferObject&& other) noexcept : 
		vboId_(other.vboId_), size_(other.size_), target_(other.target_) {

		other.vboId_ = 0;
		other.size_ = 0;
		other.target_ = 0;
	}

	VertexBufferObject& VertexBufferObject::operator=(VertexBufferObject&& other) noexcept {
		vboId_ = other.vboId_;
		size_ = other.size_;
		target_ = other.target_;

		other.vboId_ = 0;
		other.size_ = 0;
		other.target_ = 0;
		return *this;
	}

	void VertexBufferObject::bufferData(GLsizeiptr size, const GLvoid* data, GLenum usage) {
        size_ = size;

        glBufferData(target_, size, data, usage);
        checkGlError();
	}

	void VertexBufferObject::bufferSubData(GLsizeiptr offset, GLsizeiptr size, const GLvoid* data) const {
		if (vboId_ != 0 && target_ != 0) {
			glBufferSubData(target_, offset, size, data);
			checkGlError();
		} else {
		    logger()->warn("[VertexBufferObject] Calling bindSubData failed, must call bindData first");
		}
	}

    void VertexBufferObject::generate() {
        if (vboId_ == 0) {
            glGenBuffers(1, &vboId_);
            checkGlError();
		} else {
			logger()->warn("[VertexBufferObject] Calling generate failed, generate has already been called");
		}
	}

	void VertexBufferObject::bind(GLenum target) {
		if (vboId_ != 0) {
			target_ = target;
			glBindBuffer(target_, vboId_);
			checkGlError();
		} else {
			logger()->warn("[VertexBufferObject] bind failed, generate must be called first");
		}
	}

	void VertexBufferObject::unbind() const {
		glBindBuffer(target_, 0);
		checkGlError();
	}

} // Namespace sdl.
