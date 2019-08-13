#ifndef CPPSDL2_SDL_IMGUISHADER_H
#define CPPSDL2_SDL_IMGUISHADER_H

#include "shaderprogram.h"

#include <glm/mat4x2.hpp>

namespace sdl {

	class ImGuiShader {
	public:
		ImGuiShader() = default;
		ImGuiShader(const GLchar* vShade, const GLchar* fShader);

		ImGuiShader(const ImGuiShader&) = delete;
		ImGuiShader& operator=(const ImGuiShader&) = delete;

		ImGuiShader(ImGuiShader&& other) noexcept;
		ImGuiShader& operator=(ImGuiShader&& other) noexcept;

		void useProgram() const;

		void setVertexAttribPointer() const;

		void setMatrix(const glm::mat4x4& matrix) const;

		void setTextureId(GLint textureId) const;

	private:
		sdl::ShaderProgram shader_;

		// Vertex buffer attributes.
		int aPosIndex_ = -1;
		int aTexIndex_ = -1;
		int aColorIndex_ = -1;

		// Vertex buffer uniform.
		int uMatrixIndex_ = -1;
		int uTextureIndex_ = -1;
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_IMGUISHADER_H
