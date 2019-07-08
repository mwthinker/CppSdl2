#include "imguishader.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace sdl {

	ImGuiShader::ImGuiShader() : aPosIndex_(-1), aTexIndex_(-1), aColorIndex_(-1),
		uMatrixIndex_(-1), uTextureIndex_(-1) {
	}

	ImGuiShader::ImGuiShader(const GLchar* vShade, const GLchar* fShader) {
		shader_.bindAttribute("Position");
		shader_.bindAttribute("UV");
		shader_.bindAttribute("Color");
		shader_.loadAndLink(vShade, fShader);

		shader_.useProgram();

		// Collect the vertex buffer attributes indexes.
		aPosIndex_ = shader_.getAttributeLocation("Position");
		aTexIndex_ = shader_.getAttributeLocation("UV");
		aColorIndex_ = shader_.getAttributeLocation("Color");

		// Collect the vertex buffer uniforms indexes.
		uMatrixIndex_ = shader_.getUniformLocation("ProjMtx");
		uTextureIndex_ = shader_.getUniformLocation("Texture");
	}

	void ImGuiShader::useProgram() const {
		shader_.useProgram();
	}

	void ImGuiShader::setVertexAttribPointer() const {
		glEnableVertexAttribArray(aPosIndex_);
		glVertexAttribPointer(aPosIndex_, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
		sdl::checkGlError();

		glEnableVertexAttribArray(aTexIndex_);
		glVertexAttribPointer(aTexIndex_, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
		sdl::checkGlError();

		glEnableVertexAttribArray(aColorIndex_);
		glVertexAttribPointer(aColorIndex_, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));
		sdl::checkGlError();
	}

	void ImGuiShader::setMatrix(const glm::mat4x4& matrix) const {
		glUniformMatrix4fv(uMatrixIndex_, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void ImGuiShader::setTexture(bool useTexture) const {
		glUniform1i(uTextureIndex_, (int)useTexture);
	}

} // Namespace sdl.
