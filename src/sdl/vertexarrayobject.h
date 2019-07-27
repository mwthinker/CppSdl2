#ifndef CPPSDL2_SDL_VERTEXARRAYOBJECT_H
#define CPPSDL2_SDL_VERTEXARRAYOBJECT_H

#include "opengl.h"

namespace sdl {

	class VertexArrayObject {
	public:
		VertexArrayObject() noexcept;

		~VertexArrayObject();

		VertexArrayObject(const VertexArrayObject&) = delete;
		VertexArrayObject& operator=(const VertexArrayObject&) = delete;

		VertexArrayObject(VertexArrayObject&& other) noexcept;
		VertexArrayObject& operator=(VertexArrayObject&& other) noexcept;

		void generate();

		void bind() const;

		static void unbind();

	private:
		GLuint vao_;
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_VERTEXARRAYOBJECT_H
