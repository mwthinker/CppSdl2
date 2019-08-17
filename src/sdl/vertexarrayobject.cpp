#include "vertexarrayobject.h"
#include "vertexbufferobject.h"

#include "window.h"
#include "logger.h"

namespace sdl {

	VertexArrayObject::~VertexArrayObject() {
		if (vao_ != 0) {
			glDeleteVertexArrays(1, &vao_);
			logger()->debug("[VertexArrayObject] Deleted vao: {}", vao_);
			assertGlError();
		}
	}

	VertexArrayObject::VertexArrayObject(VertexArrayObject&& other) noexcept
		: vao_(other.vao_) {

		other.vao_ = 0;
	}
	
	VertexArrayObject& VertexArrayObject::operator=(VertexArrayObject&& other) noexcept {
		vao_ = other.vao_;
		
		other.vao_ = 0;
		return *this;
	}

	void VertexArrayObject::generate() {
		if (vao_ == 0) {
			glGenVertexArrays(1, &vao_);
			assertGlError();
		} else {
			logger()->warn("[VertexArrayObject] tried to create, but vao was already exists");
		}
	}

	void VertexArrayObject::bind() const {
		if (vao_ != 0) {
			glBindVertexArray(vao_);
			assertGlError();
		} else {
			logger()->debug("[VertexArrayObject] Must be generated first");
		}
	}

	void VertexArrayObject::unbind() {
		glBindVertexArray(0);
		logger()->debug("[VertexArrayObject] Unbind vao");
	}

} // Namespace mw.
