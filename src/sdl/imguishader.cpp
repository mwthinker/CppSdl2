#include "imguishader.h"
#include "logger.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace sdl {

	namespace {

		constexpr const char* A_POS = "Position";
		constexpr const char* A_TEX = "UV";
		constexpr const char* A_COL = "Color";

		constexpr const char* U_MAT = "ProjMtx";
		constexpr const char* U_TEXTURE = "Texture";

	}

	ImGuiShader::ImGuiShader() : aPosIndex_(-1), aTexIndex_(-1), aColorIndex_(-1),
		uMatrixIndex_(-1), uTextureIndex_(-1) {
	}

	ImGuiShader::ImGuiShader(const GLchar* vShade, const GLchar* fShader) : ImGuiShader() {
		shader_.bindAttribute(A_POS);
		shader_.bindAttribute(A_TEX);
		shader_.bindAttribute(A_COL);
		shader_.loadAndLink(vShade, fShader);

		shader_.useProgram();

		if (shader_.isLinked()) {
			// Collect the vertex buffer attributes indexes.
			aPosIndex_ = shader_.getAttributeLocation(A_POS);
			if (aPosIndex_ == -1) {
				logger()->warn("[ImGuiShader] shader attribute {} failed to be extracted", A_POS);
			}

			aTexIndex_ = shader_.getAttributeLocation(A_TEX);
			if (aTexIndex_ == -1) {
				logger()->warn("[ImGuiShader] shader attribute {} failed to be extracted", A_TEX);
			}

			aColorIndex_ = shader_.getAttributeLocation(A_COL);
			if (aColorIndex_ == -1) {
				logger()->warn("[ImGuiShader] shader attribute {} failed to be extracted", A_COL);
			}

			// Collect the vertex buffer uniforms indexes.
			uMatrixIndex_ = shader_.getUniformLocation(U_MAT);
			if (uMatrixIndex_ == -1) {
				logger()->warn("[ImGuiShader] shader uniform {} failed to be extracted", U_MAT);
			}
			uTextureIndex_ = shader_.getUniformLocation(U_TEXTURE);
			if (uTextureIndex_ == -1) {
				logger()->warn("[ImGuiShader] shader uniform {} failed to be extracted", U_TEXTURE);
			}
		} else {
			logger()->warn("[ImGuiShader] failed to create ImGuiShader, shader not linked");
		}
	}

	ImGuiShader::ImGuiShader(ImGuiShader&& other) noexcept :
		shader_(std::move(other.shader_)), aPosIndex_(other.aPosIndex_),
		aTexIndex_(other.aTexIndex_), aColorIndex_(other.aColorIndex_),
		uMatrixIndex_(other.uMatrixIndex_), uTextureIndex_(other.uTextureIndex_) {

		other.aPosIndex_ = -1;
		other.aTexIndex_ = -1;
		other.aColorIndex_ = -1;

		other.uMatrixIndex_ = -1;
		other.uTextureIndex_ = -1;
	}

	ImGuiShader& ImGuiShader::operator=(ImGuiShader&& other) noexcept {
		shader_ = std::move(other.shader_);
		aPosIndex_ = other.aPosIndex_;
		aTexIndex_ = other.aTexIndex_;
		aColorIndex_ = other.aColorIndex_;
		uMatrixIndex_ = other.uMatrixIndex_;
		uTextureIndex_ = other.uTextureIndex_;

		other.aPosIndex_ = -1;
		other.aTexIndex_ = -1;
		other.aColorIndex_ = -1;

		other.uMatrixIndex_ = -1;
		other.uTextureIndex_ = -1;

		return *this;
	}

	void ImGuiShader::useProgram() const {
		shader_.useProgram();
	}

	void ImGuiShader::setVertexAttribPointer() const {
		if (shader_.isLinked()) {
			glEnableVertexAttribArray(aPosIndex_);
			glVertexAttribPointer(aPosIndex_, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
			sdl::checkGlError();

			glEnableVertexAttribArray(aTexIndex_);
			glVertexAttribPointer(aTexIndex_, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
			sdl::checkGlError();

			glEnableVertexAttribArray(aColorIndex_);
			glVertexAttribPointer(aColorIndex_, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));
			sdl::checkGlError();
		} else {
			logger()->warn("[ImGuiShader] setVertexAttribPointer failed, shader not linked");
		}
	}

	void ImGuiShader::setMatrix(const glm::mat4x4& matrix) const {
		glUniformMatrix4fv(uMatrixIndex_, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void ImGuiShader::setTextureId(GLint textureId) const {
		glUniform1i(uTextureIndex_, textureId);
	}

} // Namespace sdl.
