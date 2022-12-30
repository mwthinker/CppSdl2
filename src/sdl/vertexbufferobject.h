#ifndef CPPSDL2_SDL_VERTEXBUFFEROBJECT_H
#define CPPSDL2_SDL_VERTEXBUFFEROBJECT_H

#include "opengl.h"

namespace sdl {

	class VertexBufferObject {
	public:
		// Create a empty vertex buffer object.
		VertexBufferObject() = default;

		~VertexBufferObject();

		VertexBufferObject(const VertexBufferObject&) = delete;
		VertexBufferObject& operator=(const VertexBufferObject&) = delete;

		VertexBufferObject(VertexBufferObject&& other) noexcept;
		VertexBufferObject& operator=(VertexBufferObject&& other) noexcept;

		// Generates an id or use the current one. Bind the buffer data to the graphic card. The buffer is then unbind.
		void bufferData(gl::GLsizeiptr size, const gl::GLvoid* data, gl::GLenum usage);

		// Replaces the data in the buffer. The current buffer is binded.
		// Will replace data if the buffer exists on the graphic card.
		void bufferSubData(gl::GLsizeiptr offset, gl::GLsizeiptr size, const gl::GLvoid* data);

		// Bind the current vertex buffer.
		void bind(gl::GLenum target);

		bool isGenerated() const noexcept;

		// Generate a new buffer on the graphic card first time called.
		void generate();

		// Unbind the current vertex buffer.
		void unbind();

		// Return the size in bytes for the current data.
		gl::GLsizeiptr getSize() const noexcept;

		// Return the target specified.
		gl::GLenum getTarget() const noexcept;

		gl::GLuint getId() const noexcept;

	private:
		gl::GLuint vboId_ = 0;
		gl::GLsizeiptr size_ = 0;
		gl::GLenum target_ = gl::GL_ARRAY_BUFFER;
		gl::GLenum usage_ = gl::GL_STATIC_DRAW;
	};

	inline gl::GLsizeiptr VertexBufferObject::getSize() const noexcept {
		return size_;
	}
	
	inline gl::GLenum VertexBufferObject::getTarget() const noexcept {
		return target_;
	}

	inline gl::GLuint VertexBufferObject::getId() const noexcept {
		return vboId_;
	}

}

#endif
