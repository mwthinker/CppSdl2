#ifndef CPPSDL2_SDL_SHADER_H
#define CPPSDL2_SDL_SHADER_H

#include "shaderprogram.h"

#include <glm/mat4x2.hpp>

namespace sdl {

	constexpr const GLchar* getImGuiVertexShaderGlsl_330() {
		return R"(#version 330 core

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
}
)";
	}

	constexpr const GLchar* getImGuiFragmentShaderGlsl_330() {
		return R"(#version 330 core

uniform sampler2D uTexture;
uniform float uUseTexture;

in vec2 fragTex;
in vec4 fragColor;

out vec4 Out_Color;

void main() {
    Out_Color = fragColor * (texture(uTexture, fragTex.st) * uUseTexture + (1 - uUseTexture));
}
)";
	}

	class Shader {
	public:
		Shader() = default;
		Shader(const GLchar* vShade, const GLchar* fShader);

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		Shader(Shader&& other) noexcept = default;
		Shader& operator=(Shader&& other) noexcept = default;

		void useProgram() const;

		void setVertexAttribPointer() const;

		void setMatrix(const glm::mat4& matrix) const;

		void setTextureId(GLint textureId) const;

	private:
		sdl::ShaderProgram shader_;
		
		// Vertex buffer attributes.
		GLuint aPos_{-1};
		GLuint aTex_{-1};
		GLuint aColor_{-1};

		// Vertex buffer uniforms.
		GLuint uMat_{-1};
		GLuint uTexture_{-1};
		GLuint uUseTexture_{-1};
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_SHADER_H
