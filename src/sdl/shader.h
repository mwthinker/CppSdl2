#ifndef CPPSDL2_SDL_SHADER_H
#define CPPSDL2_SDL_SHADER_H

#include "shaderprogram.h"

#include <glm/mat4x2.hpp>

namespace sdl {

	class Shader {
	public:
		Shader() = default;

		static Shader CreateShaderGlsl_330();

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		Shader(Shader&& other) noexcept = default;
		Shader& operator=(Shader&& other) noexcept = default;

		void useProgram();

		void setVertexAttribPointer();

		void setMatrix(const glm::mat4& matrix);

		void setTextureId(GLint textureId);

	private:
		Shader(const GLchar* vShade, const GLchar* fShader);

		sdl::ShaderProgram shader_;
		
		// Vertex buffer attributes.
		int aPos_{-1};
		int aTex_{-1};
		int aColor_{-1};

		// Vertex buffer uniforms.
		int uMat_{-1};
		int uTexture_{-1};
		int uUseTexture_{-1};
	};

}

#endif
