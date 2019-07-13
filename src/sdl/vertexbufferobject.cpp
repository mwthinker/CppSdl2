#include "vertexbufferobject.h"
#include "logger.h"

namespace sdl {

	VertexBufferObject::VertexBufferObject() noexcept
		: vboId_(0), size_(0), target_(0) {

	}

	VertexBufferObject::~VertexBufferObject() {
		// Opengl buffer loaded? And the opengl context active?
		if (vboId_ != 0) {
			// Is called if the buffer is valid and therefore need to be cleaned up.
			glDeleteBuffers(1, &vboId_);
			checkGlError();
			vboId_ = 0;
			size_ = 0;
			target_ = 0;
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

	void VertexBufferObject::bindData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) {
		if (vboId_ == 0) {
			target_ = target;
			size_ = size;
			
			glGenBuffers(1, &vboId_);
			glBindBuffer(target, vboId_);
			glBufferData(target, size, data, usage);
			checkGlError();
		} else {
			logger()->warn("[VertexBufferObject] BindData failed, already binded");
		}
	}

	void VertexBufferObject::bindSubData(GLsizeiptr offset, GLsizeiptr size, const GLvoid* data) const {
		if (vboId_ != 0) {
			glBindBuffer(target_, vboId_);
			glBufferSubData(target_, offset, size, data);
			checkGlError();
		} else {
			logger()->warn("[VertexBufferObject] bindSubData failed, data not binded");
		}
	}

	void VertexBufferObject::bind() const {
		if (vboId_ != 0) {
			glBindBuffer(target_, vboId_);
			checkGlError();
		} else {
			logger()->warn("[VertexBufferObject] bind failed, bindData must be called first");
		}
	}

	void VertexBufferObject::unbind() const {
		glBindBuffer(target_, 0);
		checkGlError();
	}

} // Namespace sdl.
