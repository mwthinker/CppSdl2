#include "shader.h"
#include "vertex.h"

#include <spdlog/spdlog.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace sdl {

	namespace {

		constexpr const gl::GLchar* aPos = "aPos";
		constexpr const gl::GLchar* aTex = "aTex";
		constexpr const gl::GLchar* aCol = "aColor";

		constexpr const gl::GLchar* uMat = "uMat";
		constexpr const gl::GLchar* uTexture = "uTexture";
		constexpr const gl::GLchar* uUseTexture = "uUseTexture";

		constexpr void vertexEqualImDrawVert() {
			static_assert(sizeof(ImDrawVert::col) == sizeof(Vertex::color));
			static_assert(sizeof(ImDrawVert::pos) == sizeof(Vertex::pos));
			static_assert(sizeof(ImDrawVert::uv) == sizeof(Vertex::tex));
			static_assert(sizeof(ImDrawVert) == sizeof(Vertex));
		}

		constexpr const gl::GLchar* VertexShaderGlsl_330 =
R"(#version 330 core

uniform mat4 uMat;

in vec2 aPos;
in vec2 aTex;
in vec4 aColor;

out vec2 fragTex;
out vec4 fragColor;

void main() {
	fragTex = aTex;
	fragColor = aColor;
	gl_Position = uMat * vec4(aPos.xy, 0, 1);
	gl_PointSize = aTex.x;
}
)";

		constexpr const gl::GLchar* FragmentShaderGlsl_330 =
R"(#version 330 core

uniform sampler2D uTexture;
uniform float uUseTexture;

in vec2 fragTex;
in vec4 fragColor;

out vec4 oColor;

void main() {
	oColor = fragColor * (texture(uTexture, fragTex.st) * uUseTexture + (1 - uUseTexture));
}
)";

	}

	Shader Shader::CreateShaderGlsl_330() {
		return Shader{VertexShaderGlsl_330, FragmentShaderGlsl_330};
	}

	Shader::Shader(const gl::GLchar* vShade, const gl::GLchar* fShader) {
		shader_.bindAttribute(aPos);
		shader_.bindAttribute(aTex);
		shader_.bindAttribute(aCol);

		if (shader_.loadAndLink(vShade, fShader)) {
			// Collect the vertex buffer attributes indexes.
			aPos_ = shader_.getAttributeLocation(aPos);
			aTex_ = shader_.getAttributeLocation(aTex);
			aColor_ = shader_.getAttributeLocation(aCol);

			// Collect the vertex buffer uniforms indexes.
			uMat_ = shader_.getUniformLocation(uMat);
			uTexture_ = shader_.getUniformLocation(uTexture);
			uUseTexture_ = shader_.getUniformLocation(uUseTexture);
		} else {
			spdlog::warn("[sdl::Shader] failed to create VinShader, shader not linked");
		}
	}

	void Shader::useProgram() {
		shader_.useProgram();
	}

	void Shader::setVertexAttribPointer() {
		if (shader_.isLinked()) {
			gl::glEnableVertexAttribArray(aPos_);
			gl::glVertexAttribPointer(aPos_, sizeof(Vertex::pos) / sizeof(gl::GLfloat) , gl::GL_FLOAT, gl::GL_FALSE, sizeof(Vertex), (gl::GLvoid*) offsetof(Vertex, pos));

			gl::glEnableVertexAttribArray(aTex_);
			gl::glVertexAttribPointer(aTex_, sizeof(Vertex::tex) / sizeof(gl::GLfloat), gl::GL_FLOAT, gl::GL_FALSE, sizeof(Vertex), (gl::GLvoid*) offsetof(Vertex, tex));

			gl::glEnableVertexAttribArray(aColor_);
			gl::glVertexAttribPointer(aColor_, 4, gl::GL_UNSIGNED_BYTE, gl::GL_TRUE, sizeof(Vertex), (gl::GLvoid*) offsetof(Vertex, color));
		} else {
			spdlog::warn("[sdl::Shader] setVertexAttribPointer failed, shader not linked");
		}
	}

	void Shader::setMatrix(const glm::mat4& matrix) {
		gl::glUniformMatrix4fv(uMat_, 1, gl::GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::setTextureId(int textureId) {
		if (textureId < 0) {
			gl::glUniform1f(uUseTexture_, 0.f);
		} else {
			gl::glUniform1i(uTexture_, textureId);
			gl::glUniform1f(uUseTexture_, 1.f);
		}
	}

}
