#ifndef CPPSDL2_SDL_SHADER_H
#define CPPSDL2_SDL_SHADER_H

#include "opengl.h"

#include <memory>
#include <string>
#include <map>

namespace sdl {

	class ShaderObject {
	public:
		// Create an empty non linked shader.
		ShaderObject();

		ShaderObject(const ShaderObject& sprite) = delete;
		ShaderObject& operator=(const ShaderObject& sprite) = delete;

		ShaderObject(ShaderObject&& sprite) noexcept;
		ShaderObject& operator=(ShaderObject&& sprite) noexcept;

		~ShaderObject();

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
		bool isLinked() const {
			return programObjectId_ != 0;
		}

	private:
		static int currentProgramId;
		
		mutable std::map<std::string, int> attributes_;
		mutable std::map<std::string, int> uniforms_;

		int location_;
		GLuint programObjectId_;
		int windowInstance_;
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_SHADER_H
