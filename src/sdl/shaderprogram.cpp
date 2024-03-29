#include "shaderprogram.h"

#include <spdlog/spdlog.h>

#include <fstream>
#include <sstream>

namespace sdl {

	namespace {

		enum class LogError {
			ProgramError,
			ShaderError
		};

		template <LogError Type>
		void logError(gl::GLuint objectId, const char* shaderString, const char* errorHeader) {
			gl::GLint infoLen = 0;
			if constexpr (Type == LogError::ProgramError) {
				gl::glGetProgramiv(objectId, gl::GL_INFO_LOG_LENGTH, &infoLen);
			} else {
				gl::glGetShaderiv(objectId, gl::GL_INFO_LOG_LENGTH, &infoLen);
			}
			if (infoLen > 1) {
				char message[256]; // A arbitrary value big enough to contain message.
				gl::GLsizei size = 0;
				if constexpr (Type == LogError::ProgramError) {
					gl::glGetProgramInfoLog(objectId, sizeof(message), &size, message);
				} else {
					gl::glGetShaderInfoLog(objectId, sizeof(message), &size, message);
				}
				spdlog::error("[sdl::ShaderProgram] {}, {}: {}", shaderString, errorHeader, std::string_view(message, size));
			}
		}

		gl::GLuint loadShader(gl::GLuint program, gl::GLenum type, const gl::GLchar* shaderSrc) {
			auto shader= glCreateShader(type);
			if (shader == 0) {
				spdlog::error("[sdl::ShaderProgram] Failed to create shader: ", shaderSrc);
				return 0;
			}
			gl::glShaderSource(shader, 1, &shaderSrc, nullptr);
			gl::glCompileShader(shader);

			gl::GLint compileStatus;
			gl::glGetShaderiv(shader, gl::GL_COMPILE_STATUS, &compileStatus);
			if (!compileStatus) {
				logError<LogError::ShaderError>(shader, shaderSrc, "Failed to compile shader: ");
				return 0;
			}
			gl::glAttachShader(program, shader);
			return shader;
		}

		std::string loadFromFile(const std::string& file) {
			if (file.empty()) {
				return "";
			}
			std::ifstream inFile{file};
			std::stringstream stream;
			stream << inFile.rdbuf();
			return stream.str();
		}

	}

	ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept :
		attributes_{std::move(other.attributes_)},
		uniforms_{std::move(other.uniforms_)},
		programObjectId_{std::exchange(other.programObjectId_, 0)} {
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
		if (programObjectId_ != 0) {
			gl::glDeleteProgram(programObjectId_);
		}
		
		attributes_ = std::move(other.attributes_);
		uniforms_ = std::move(other.uniforms_);
		programObjectId_ = std::exchange(other.programObjectId_, 0);
		return *this;
	}

	ShaderProgram::~ShaderProgram() {
		if (programObjectId_ != 0) {
			gl::glDeleteProgram(programObjectId_);
		}
	}

	void ShaderProgram::bindAttribute(const std::string& attribute) {
		if (programObjectId_ == 0) {
			attributes_[attribute] = 0;
		} else {
			spdlog::warn("[sdl::ShaderProgram] Failed to bind attribute, program is already compiled");
		}
	}

	int ShaderProgram::getAttributeLocation(const std::string& attribute) const {
		auto it = attributes_.find(attribute);
		if (it != attributes_.end()) {
			return it->second;
		}
		spdlog::warn("[sdl::ShaderProgram] shader attribute {} failed to be extracted", attribute);
		return -1;
	}

	int ShaderProgram::getUniformLocation(const std::string& uniform) const {
		if (programObjectId_ == 0) {
			spdlog::warn("[sdl::ShaderProgram] shader uniform {} failed to be extracted", uniform);
			return -1;
		}

		if (auto it = uniforms_.find(uniform); 
			it != uniforms_.end()) {
			
			return it->second;
		}

		auto loc = gl::glGetUniformLocation(programObjectId_, uniform.c_str());
		if (loc != -1) {
			uniforms_[uniform] = loc;
		}
		return loc;
	}

	bool ShaderProgram::loadAndLinkFromFile(const std::string& vShaderFile, const std::string& gShaderFile, const std::string& fShaderFile) {
		return loadAndLink(loadFromFile(vShaderFile), loadFromFile(gShaderFile), loadFromFile(fShaderFile));
	}

	bool ShaderProgram::loadAndLinkFromFile(const std::string& vShaderFile, const std::string& fShaderFile) {
		return loadAndLinkFromFile(vShaderFile, "", fShaderFile);
	}

	bool ShaderProgram::loadAndLink(const std::string& vShader, const std::string& gShader, const std::string& fShader) {
		if (programObjectId_ != 0) {
			spdlog::warn("[sdl::ShaderProgram] Failed to load and link, opengl program already generated");
			return false;
		}
		
		programObjectId_ = gl::glCreateProgram();
			
		if (programObjectId_ == 0) {
			spdlog::error("[sdl::ShaderProgram] Failed to create program");
			return false;
		}

		if (0 == loadShader(programObjectId_, gl::GL_VERTEX_SHADER, vShader.c_str())) {
			return false;
		}
		if (!gShader.empty() && 0 == loadShader(programObjectId_, gl::GL_GEOMETRY_SHADER, gShader.c_str())) {
			return false;
		}
		if (0 == loadShader(programObjectId_, gl::GL_FRAGMENT_SHADER, fShader.c_str())) {
            return false;
		}

		bindAllAttributes();

		if (!linkProgram()) {
			return false;
		}

		useProgram();
		return true;
	}

	bool ShaderProgram::loadAndLink(const std::string& vShader, const std::string& fShader) {
		return loadAndLink(vShader, "", fShader);
	}

	void ShaderProgram::useProgram() {
		if (programObjectId_ != 0) {
			gl::glUseProgram(programObjectId_);
		} else {
			spdlog::warn("[sdl::ShaderProgram] Failed to use program, program is not compiled");
		}
	}
	
	bool ShaderProgram::linkProgram() {
		gl::glLinkProgram(programObjectId_);
		gl::GLint linked;
		gl::glGetProgramiv(programObjectId_, gl::GL_LINK_STATUS, &linked);
		if (!linked) {
			logError<LogError::ProgramError>(programObjectId_, "", "Error linking program");
			gl::glDeleteProgram(programObjectId_);
			return false;
		}
		return true;
	}	

	void ShaderProgram::bindAllAttributes() {
		int index = 0;
		for (auto& [name, location] : attributes_) {
			gl::glBindAttribLocation(programObjectId_, index, name.c_str());
			location = index++;
		}
	}

}
