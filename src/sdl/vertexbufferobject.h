#ifndef CPPSDL2_SDL_VERTEXBUFFEROBJECT_H
#define CPPSDL2_SDL_VERTEXBUFFEROBJECT_H

#include "opengl.h"

namespace sdl {

	class VertexBufferObject {
	public:
		// Create a empty vertex buffer object.
		VertexBufferObject() noexcept;

		~VertexBufferObject();

		VertexBufferObject(const VertexBufferObject&) = delete;
		VertexBufferObject& operator=(const VertexBufferObject&) = delete;

		VertexBufferObject(VertexBufferObject&& other) noexcept;
		VertexBufferObject& operator=(VertexBufferObject&& other) noexcept;
				
		// Generates an id and bind the buffer data to the graphic card. The buffer is then unbind.
		// Will only bind for the first call to this function.
		void bindData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);

		// Replaces the data in the buffer. The current buffer is binded.
		// Will replace data if the buffer exists on the graphic card.
		void bindSubData(GLsizeiptr offset, GLsizeiptr size, const GLvoid* data) const;

		// Bind the current vertex buffer with the initial target.
		void bind() const;

		// Unbind the current vertex buffer.
		void unbind() const;

		// Return the size in bytes for the current data.
		GLsizeiptr getSize() const noexcept;

		// Return the target specified.
		GLenum getTarget() const noexcept;

	private:
		GLuint vboId_;
		GLsizeiptr size_;
		GLenum target_;
	};

	inline GLsizeiptr VertexBufferObject::getSize() const noexcept {
		return size_;
	}
	
	inline GLenum VertexBufferObject::getTarget() const noexcept {
		return target_;
	}

} // Namespace sdl.

#endif // CPPSDL2_SDL_VERTEXBUFFEROBJECT_H
