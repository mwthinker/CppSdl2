#ifndef CPPSDL2_SDL_SHADERPROGRAM_H
#define CPPSDL2_SDL_SHADERPROGRAM_H

#include "opengl.h"

#include <string>
#include <map>

namespace sdl {

	class ShaderProgram {
	public:
		// Create an empty non linked shader.
		ShaderProgram() = default;

		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

		ShaderProgram(ShaderProgram&& other) noexcept;
		ShaderProgram& operator=(ShaderProgram&& other) noexcept;

		~ShaderProgram();

		// Bind the attribute to the shader.
		// Must be called before linking the shader in order for the attribute to
		// be available. I.e. before calling loadAndLinkShadersFromFile(...).
		void bindAttribute(const std::string& attribute);

		// Return the gl memory location for the attribute.
		// Return -1 on error.
		int getAttributeLocation(const std::string& attribute) const;

		// Return the gl memory location for the uniform.
		// Return -1 on error.
		int getUniformLocation(const std::string& uniform) const;

		// Load shaders from files. Is safe to call multiple times but only the 
		// first successful is "used. Load and link the vertex shader and the 
		// fragment shader in the created program. Return true if the creation
		// is successful else false.
		bool loadAndLinkFromFile(const std::string& vShaderFile, const std::string& fShaderFile);

		bool loadAndLinkFromFile(const std::string& vShaderFile, const std::string& gShaderFile, const std::string& fShaderFile);

		// Load shaders string parameters "vShader" and "fShader". Is safe to 
		// call multiple times but only the first successful is "used. Load and
		// link the vertex shader and the fragment shader in the created program.
		// Return true if the creation is successful else false.
		bool loadAndLink(const std::string& vShader, const std::string& fShader);

		bool loadAndLink(const std::string& vShader, const std::string& gShader, const std::string& fShader);

		// Uses the current gl program. I.e. a call to glUseProgram.
		// Does nothing if the program is not loaded.
		void useProgram() const;

		// Return if the shader program is linked.
		bool isLinked() const noexcept {
			return programObjectId_ != 0;
		}

	private:
		bool linkProgram();

		void bindAllAttributes();

		std::map<std::string, GLuint> attributes_;
		mutable std::map<std::string, GLuint> uniforms_;
		
		GLuint programObjectId_ = 0;
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_SHADERPROGRAM_H
