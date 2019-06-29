#include "vertexarrayobject.h"
#include "vertexbufferobject.h"

#include "window.h"
#include "logger.h"

namespace sdl {

	VertexArrayObject::VertexArrayObject() noexcept
		: created_(false), vao_(0) {
	}

	VertexArrayObject::~VertexArrayObject() {
		if (created_) {
			glDeleteVertexArrays(1, &vao_);
			logger()->debug("[VertexArrayObject] Deleted vao: {}", vao_);
			checkGlError();
		}
	}

	VertexArrayObject::VertexArrayObject(VertexArrayObject&& other) noexcept
		: created_(other.created_), vao_(other.vao_) {

		other.created_ = false;
	}
	
	VertexArrayObject& VertexArrayObject::operator=(VertexArrayObject&& other) noexcept {
		vao_ = other.vao_;
		
		other.created_ = false;
		return *this;
	}

	void VertexArrayObject::create() {
		if (!created_) {
			created_ = true;
			glGenVertexArrays(1, &vao_);
			glBindVertexArray(vao_);
			checkGlError();
		} else {
			logger()->warn("[VertexArrayObject] tried to create, but vao was already exists");
		}
	}

	void VertexArrayObject::bind() const {
		if (created_) {
			glBindVertexArray(vao_);
			checkGlError();
		} else {
			logger()->debug("[VertexArrayObject] ");
		}
	}

	void VertexArrayObject::unbind() {
		glBindVertexArray(0);
		logger()->debug("[VertexArrayObject] Unbind vao");
	}

} // Namespace mw.
