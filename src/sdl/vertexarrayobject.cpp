#include "vertexarrayobject.h"
#include "vertexbufferobject.h"

#include "window.h"

namespace sdl {

	GLuint VertexArrayObject::currentVertexArrayObject = -1;

	VertexArrayObject::VertexArrayObject() : vaoSupported_(VaoSupported::CHECK_SUPPORTED), vao_(std::make_shared<GLuint>(0)) {
	}

	VertexArrayObject::~VertexArrayObject() {
		if (currentVertexArrayObject == *vao_) {
			currentVertexArrayObject = 0;
		}
		if (*vao_ > 0) {
			glDeleteVertexArrays(1, &*vao_);
			checkGlError();
		}
		*vao_ = 0;
	}

	void VertexArrayObject::bind() const {
		useProgram();
		switch (vaoSupported_) {
			case VaoSupported::CHECK_SUPPORTED:
				if (Window::getOpenGlMajorVersion() >= 3) {
					vaoSupported_ = VaoSupported::SUPPORTED;
					// Fall trough to next case.
				} else {
					vaoSupported_ = VaoSupported::NOT_SUPPORTED;
					bindBuffer();
					setVertexAttribPointer();
					break;
				}
			case VaoSupported::SUPPORTED:
				if (*vao_ == 0) { // Create the vertex array only the first time.
					glGenVertexArrays(1, &*vao_);
					glBindVertexArray(*vao_);
					checkGlError();
					// Remove the block to unneeded calls to buffer changes.
					VertexBufferObject::setIgnoreCurrentBind(true);
					bindBuffer();
					// Restore the block.
					VertexBufferObject::setIgnoreCurrentBind(false);
					currentVertexArrayObject = *vao_;
					setVertexAttribPointer();
				} else {
					glBindVertexArray(*vao_);
					checkGlError();
				}
				break;
			case VaoSupported::NOT_SUPPORTED:
				bindBuffer();
				setVertexAttribPointer();
				break;
		}
	}

	void VertexArrayObject::unbind() const {
		glBindVertexArray(0);
	}

} // Namespace mw.
