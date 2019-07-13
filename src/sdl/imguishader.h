#ifndef CPPSDL2_SDL_IMGUISHADER_H
#define CPPSDL2_SDL_IMGUISHADER_H

#include "shaderprogram.h"

#include <glm/mat4x2.hpp>

namespace sdl {

	class ImGuiShader {
	public:
		ImGuiShader();
		ImGuiShader(const GLchar* vShade, const GLchar* fShader);

		ImGuiShader(const ImGuiShader& sprite) = delete;
		ImGuiShader& operator=(const ImGuiShader& sprite) = delete;

		ImGuiShader(ImGuiShader&& other) noexcept;
		ImGuiShader& operator=(ImGuiShader&& other) noexcept;

		void useProgram() const;

		void setVertexAttribPointer() const;

		void setMatrix(const glm::mat4x4& matrix) const;

		void setTexture(bool useTexture) const;

	private:
		sdl::ShaderProgram shader_;

		// Vertex buffer attributes.
		int aPosIndex_;
		int aTexIndex_;
		int aColorIndex_;

		// Vertex buffer uniform.
		int uMatrixIndex_;
		int uTextureIndex_;
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_IMGUISHADER_H
