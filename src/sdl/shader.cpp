#include "shader.h"
#include "vertex.h"

#include <spdlog/spdlog.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace sdl {

	namespace {

		constexpr const GLchar* A_POS = "aPos";
		constexpr const GLchar* A_TEX = "aTex";
		constexpr const GLchar* A_COL = "aColor";

		constexpr const GLchar* U_MAT = "uMat";
		constexpr const GLchar* U_TEXTURE = "uTexture";
		constexpr const GLchar* U_USE_TEXTURE = "uUseTexture";

		constexpr void VERTEX_EQUAL_IMDRAWVERT() {
			static_assert(sizeof(ImDrawVert::col) == sizeof(Vertex::color));
			static_assert(sizeof(ImDrawVert::pos) == sizeof(Vertex::pos));
			static_assert(sizeof(ImDrawVert::uv) == sizeof(Vertex::tex));
			static_assert(sizeof(ImDrawVert) == sizeof(Vertex));
		}

	}

	Shader::Shader(const GLchar* vShade, const GLchar* fShader) {
		shader_.bindAttribute(A_POS);
		shader_.bindAttribute(A_TEX);
		shader_.bindAttribute(A_COL);
		shader_.loadAndLink(vShade, fShader);

		shader_.useProgram();

		if (shader_.isLinked()) {
			// Collect the vertex buffer attributes indexes.
			aPos_ = shader_.getAttributeLocation(A_POS);
			aTex_ = shader_.getAttributeLocation(A_TEX);
			aColor_ = shader_.getAttributeLocation(A_COL);

			// Collect the vertex buffer uniforms indexes.
			uMat_ = shader_.getUniformLocation(U_MAT);
			uTexture_ = shader_.getUniformLocation(U_TEXTURE);
			uUseTexture_ = shader_.getUniformLocation(U_USE_TEXTURE);
		} else {
			spdlog::warn("[sdl::Shader] failed to create VinShader, shader not linked");
		}
	}

	void Shader::useProgram() const {
		shader_.useProgram();
	}

	void Shader::setVertexAttribPointer() const {
		if (shader_.isLinked()) {
			glEnableVertexAttribArray(aPos_);
			glVertexAttribPointer(aPos_, sizeof(Vertex::pos) / sizeof(GLfloat) , GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, pos));
			sdl::assertGlError();

			glEnableVertexAttribArray(aTex_);
			glVertexAttribPointer(aTex_, sizeof(Vertex::tex) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, tex));
			sdl::assertGlError();

			glEnableVertexAttribArray(aColor_);
			glVertexAttribPointer(aColor_, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, color));
			sdl::assertGlError();
		} else {
			spdlog::warn("[sdl::Shader] setVertexAttribPointer failed, shader not linked");
		}
	}

	void Shader::setMatrix(const glm::mat4& matrix) const {
		glUniformMatrix4fv(uMat_, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::setTextureId(GLint textureId) const {
		if (textureId < 0) {
			glUniform1f(uUseTexture_, 0.f);
		} else {
			glUniform1i(uTexture_, textureId);
			glUniform1f(uUseTexture_, 1.f);
		}
	}

}
