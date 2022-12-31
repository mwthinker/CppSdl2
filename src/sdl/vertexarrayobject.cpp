#include "vertexarrayobject.h"
#include "vertexbufferobject.h"

#include "window.h"

#include <spdlog/spdlog.h>

namespace sdl {

	VertexArrayObject::~VertexArrayObject() {
		if (vao_ != 0) {
			gl::glDeleteVertexArrays(1, &vao_);
			spdlog::debug("[sdl::VertexArrayObject] Deleted vao: {}", vao_);
		}
	}

	VertexArrayObject::VertexArrayObject(VertexArrayObject&& other) noexcept
		: vao_{std::exchange(other.vao_, 0)} {
	}
	
	VertexArrayObject& VertexArrayObject::operator=(VertexArrayObject&& other) noexcept {
		if (vao_ != 0) {
			gl::glDeleteVertexArrays(1, &vao_);
			spdlog::debug("[sdl::VertexArrayObject] Deleted vao: {}", vao_);
		}
		vao_ = std::exchange(other.vao_, 0);
		return *this;
	}

	void VertexArrayObject::generate() {
		if (vao_ == 0) {
			gl::glGenVertexArrays(1, &vao_);
		} else {
			spdlog::warn("[sdl::VertexArrayObject] tried to create, but vao already exists");
		}
	}

	void VertexArrayObject::bind() {
		if (vao_ != 0) {
			gl::glBindVertexArray(vao_);
		} else {
			spdlog::debug("[sdl::VertexArrayObject] Must be generated first");
		}
	}

	void VertexArrayObject::unbind() {
		gl::glBindVertexArray(0);
		spdlog::debug("[sdl::VertexArrayObject] Unbind vao");
	}

}
