#include "vertexbufferobject.h"

#include "window.h"

namespace sdl {

	int VertexBufferObject::currentBufferIdBinded = 0;
	bool VertexBufferObject::ignoreCurrentIdBinded = false;

	void VertexBufferObject::setIgnoreCurrentBind(bool activate) {
		ignoreCurrentIdBinded = activate;
	}

	bool VertexBufferObject::getIgnoreCurrentBind() {
		return ignoreCurrentIdBinded;
	}

	VertexBufferObject::VertexBufferObject() : data_(std::make_shared<Data>()) {
		data_->size_ = 0;
		data_->vboId_ = 0;
		data_->target_ = 0;
	}

	void VertexBufferObject::bindBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) {
		if (data_->vboId_ == 0) {
			data_->target_ = target;
			data_->size_ = size;
			
			glGenBuffers(1, &data_->vboId_);
			glBindBuffer(target, data_->vboId_);
			glBufferData(target, size, data, usage);
			checkGlError();

			currentBufferIdBinded = data_->vboId_;
			data_->windowInstance_ = Window::getInstanceId();
		}
	}

	void VertexBufferObject::bindBufferSubData(GLsizeiptr offset, GLsizeiptr size, const GLvoid* data) const {
		bindBuffer();
		if (data_->vboId_ != 0) {
			glBufferSubData(data_->target_, offset, size, data);
			checkGlError();
		}
	}

	void VertexBufferObject::bindBuffer() const {
		if (data_->vboId_ != 0 && (ignoreCurrentIdBinded || currentBufferIdBinded != data_->vboId_)) {
			glBindBuffer(data_->target_, data_->vboId_);
			checkGlError();
			currentBufferIdBinded = data_->vboId_;
		}
	}

	void VertexBufferObject::unbindBuffer() const {
		currentBufferIdBinded = 0;
		glBindBuffer(data_->target_, 0);
		checkGlError();
	}

	// Return the size in bytes for the current data.
	GLsizeiptr VertexBufferObject::getSize() const {
		return data_->size_;
	}

	// Return the target specified.
	GLenum VertexBufferObject::getTarget() const {
		return data_->target_;
	}

	VertexBufferObject::Data::Data() : vboId_(0), windowInstance_(0), size_(0), target_(0) {
	}

	VertexBufferObject::Data::~Data() {
		// Opengl buffer loaded? And the opengl context active?
		if (vboId_ != 0 && windowInstance_ == Window::getInstanceId()) {
			// Is called if the buffer is valid and therefore need to be cleaned up.
			glDeleteBuffers(1, &vboId_);
			checkGlError();
			vboId_ = 0;
			size_ = 0;
			target_ = 0;
		}
	}

} // Namespace sdl.
