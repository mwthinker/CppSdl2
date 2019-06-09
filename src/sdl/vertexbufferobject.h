#ifndef CPPSDL2_SDL_VERTEXBUFFEROBJECT_H
#define CPPSDL2_SDL_VERTEXBUFFEROBJECT_H

#include "opengl.h"

#include <memory>

namespace sdl {

	class VertexBufferObject {
	public:
		// Create a empty vertex buffer object.
		VertexBufferObject();
		
		bool operator==(const VertexBufferObject& shader) const;

		bool operator!=(const VertexBufferObject& shader) const;

		// Generates an id and bind the buffer data to the graphic card. The buffer is then unbind.
		// Will only bind for the first call to this function.
		void bindBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);

		// Replaces the data in the buffer. The current buffer is binded.
		// Will replace data if the buffer exists on the graphic card.
		void bindBufferSubData(GLsizeiptr offset, GLsizeiptr size, const GLvoid* data) const;

		// Bind the current vertex buffer with the initial target.
		void bindBuffer() const;

		// Unbind the current vertex buffer.
		void unbindBuffer() const;

		// Return the size in bytes for the current data.
		GLsizeiptr getSize() const;

		// Return the target specified.
		GLenum getTarget() const;

		static void setIgnoreCurrentBind(bool activate);
		static bool getIgnoreCurrentBind();

	private:
		static int currentBufferIdBinded;
		static bool ignoreCurrentIdBinded;

		class Data {
		public:
			Data();
			~Data();

			GLuint vboId_;
			int windowInstance_;
			GLsizeiptr size_;
			GLenum target_;
		};

		std::shared_ptr<Data> data_;
	};

	inline bool VertexBufferObject::operator==(const VertexBufferObject& vbo) const {
		return data_ == vbo.data_;
	}

	inline bool VertexBufferObject::operator!=(const VertexBufferObject& other) const {
		return !(*this == other);
	}

} // Namespace sdl.

#endif // CPPSDL2_SDL_VERTEXBUFFEROBJECT_H
