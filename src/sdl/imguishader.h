#ifndef CPPSDL2_SDL_IMGUISHADER_H
#define CPPSDL2_SDL_IMGUISHADER_H

#include "shaderprogram.h"

#include <glm/mat4x2.hpp>

namespace sdl {

	constexpr const GLchar* getImGuiVertexShaderGlsl_330() {
		return
			"#version 330 core                                           \n"
			"                                                            \n"
			"uniform mat4 ProjMtx;                                       \n"
			"                                                            \n"
			"in vec2 Position;                                           \n"
			"in vec2 UV;                                                 \n"
			"in vec4 Color;                                              \n"
			"                                                            \n"
			"out vec2 Frag_UV;                                           \n"
			"out vec4 Frag_Color;                                        \n"
			"                                                            \n"
			"void main()                                                 \n"
			"{                                                           \n"
			"    Frag_UV = UV;                                           \n"
			"    Frag_Color = Color;                                     \n"
			"    gl_Position = ProjMtx * vec4(Position.xy,0,1);          \n"
			"}                                                           \n";
	}

	constexpr const GLchar* getImGuiFragmentShaderGlsl_330() {
		return
			"#version 330 core                                           \n"
			"                                                            \n"
			"uniform sampler2D Texture;                                  \n"
			"                                                            \n"
			"in vec2 Frag_UV;                                            \n"
			"in vec4 Frag_Color;                                         \n"
			"                                                            \n"
			"out vec4 Out_Color;                                         \n"
			"                                                            \n"
			"void main()                                                 \n"
			"{                                                           \n"
			"    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);  \n"
			"}                                                           \n";
	}

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
		
		GLuint aPosIndex_ = -1;
		GLuint aTexIndex_ = -1;
		GLuint aColorIndex_ = -1;
		
		GLuint uMatrixIndex_ = -1;
		GLuint uTextureIndex_ = -1;
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_IMGUISHADER_H
