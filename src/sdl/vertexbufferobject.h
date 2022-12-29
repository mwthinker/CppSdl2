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
		void bufferData(GLsizeiptr size, const GLvoid* data, GLenum usage);

		// Replaces the data in the buffer. The current buffer is binded.
		// Will replace data if the buffer exists on the graphic card.
		void bufferSubData(GLsizeiptr offset, GLsizeiptr size, const GLvoid* data);

		// Bind the current vertex buffer.
		void bind(GLenum target);

		bool isGenerated() const noexcept;

		// Generate a new buffer on the graphic card first time called.
		void generate();

		// Unbind the current vertex buffer.
		void unbind();

		// Return the size in bytes for the current data.
		GLsizeiptr getSize() const noexcept;

		// Return the target specified.
		GLenum getTarget() const noexcept;

		GLuint getId() const noexcept;

	private:
		GLuint vboId_ = 0;
		GLsizeiptr size_ = 0;
		GLenum target_ = 0;
		GLenum usage_ = 0;
	};

	inline GLsizeiptr VertexBufferObject::getSize() const noexcept {
		return size_;
	}
	
	inline GLenum VertexBufferObject::getTarget() const noexcept {
		return target_;
	}

	inline GLuint VertexBufferObject::getId() const noexcept {
		return vboId_;
	}

}

#endif
