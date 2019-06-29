#ifndef MW_VERTEXARRAYOBJECT_H
#define MW_VERTEXARRAYOBJECT_H

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

		void create();

		void bind() const;

		static void unbind();

	private:
		bool created_;
		GLuint vao_;
	};

} // Namespace mw.

#endif // MW_VERTEXARRAYOBJECT_H
