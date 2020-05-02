#include "shaderprogram.h"

#include <spdlog/spdlog.h>

#include <fstream>
#include <sstream>

namespace sdl {

	namespace {

		enum class LogError {
			PROGRAM_ERROR,
			SHADER_ERROR
		};

		template <LogError TYPE>
		void logError(GLuint objectId, const char* shaderString, const char* errorHeader) {
			GLint infoLen = 0;
			if constexpr (TYPE == LogError::PROGRAM_ERROR) {
				glGetProgramiv(objectId, GL_INFO_LOG_LENGTH, &infoLen);
			} else {
				glGetShaderiv(objectId, GL_INFO_LOG_LENGTH, &infoLen);
			}
			if (infoLen > 1) {
				char message[256]; // A arbitrary value big enough to contain message.
				GLsizei size;
				if constexpr (TYPE == LogError::PROGRAM_ERROR) {
					glGetProgramInfoLog(objectId, sizeof(message), &size, message);
				} else {
					glGetShaderInfoLog(objectId, sizeof(message), &size, message);
				}
				std::string str;
				str.append(message, message + size);
				spdlog::error("[sdl::ShaderProgram] {}, {}: {}", shaderString, errorHeader, str);
			}
		}

		GLuint loadShader(GLuint program, GLenum type, const GLchar* shaderSrc) {
			auto shader= glCreateShader(type);
			if (shader == 0) {
				spdlog::error("[sdl::ShaderProgram] Failed to create shader: ", shaderSrc);
				return 0;
			}
			glShaderSource(shader, 1, &shaderSrc, 0);
			assertGlError();
			glCompileShader(shader);

			GLint compileStatus;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
			if (compileStatus == GL_FALSE) {
				logError<LogError::SHADER_ERROR>(shader, shaderSrc, "Failed to compile shader: ");
				return 0;
			}
			assertGlError();
			glAttachShader(program, shader);
			assertGlError();
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
			glDeleteProgram(programObjectId_);
			assertGlError();
		}
		
		attributes_ = std::move(other.attributes_);
		uniforms_ = std::move(other.uniforms_);
		programObjectId_ = std::exchange(other.programObjectId_, 0);
		return *this;
	}

	ShaderProgram::~ShaderProgram() {
		if (programObjectId_ != 0) {
			glDeleteProgram(programObjectId_);
			assertGlError();
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
		if (programObjectId_ != 0) {
			auto it = uniforms_.find(uniform);

			if (it != uniforms_.end()) {
				return it->second;
			} else {
				auto loc = glGetUniformLocation(programObjectId_, uniform.c_str());
				assertGlError();
				if (loc != -1) {
					uniforms_[uniform] = loc;
				}
				return loc;
			}
		}
		spdlog::warn("[sdl::ShaderProgram] shader uniform {} failed to be extracted", uniform);
		return -1;
	}

	bool ShaderProgram::loadAndLinkFromFile(const std::string& vShaderFile, const std::string& gShaderFile, const std::string& fShaderFile) {
		return loadAndLink(loadFromFile(vShaderFile), loadFromFile(gShaderFile), loadFromFile(fShaderFile));
	}

	bool ShaderProgram::loadAndLinkFromFile(const std::string& vShaderFile, const std::string& fShaderFile) {
		return loadAndLinkFromFile(vShaderFile, "", fShaderFile);
	}

	bool ShaderProgram::loadAndLink(const std::string& vShader, const std::string& gShader, const std::string& fShader) {
		if (programObjectId_ == 0) {
			programObjectId_ = glCreateProgram();
			assertGlError();
			
			if (programObjectId_ == 0) {
				spdlog::error("[sdl::ShaderProgram] Failed to create program");
				return false;
			}

			if (0 == loadShader(programObjectId_, GL_VERTEX_SHADER, vShader.c_str())) {
			    return false;
			}
			if (!gShader.empty() && 0 == loadShader(programObjectId_, GL_GEOMETRY_SHADER, gShader.c_str())) {
                return false;
			}
			if (0 == loadShader(programObjectId_, GL_FRAGMENT_SHADER, fShader.c_str())) {
                return false;
			}

			bindAllAttributes();

			if (!linkProgram()) {
				return false;
			}

			useProgram();
			return true;
		}
		spdlog::warn("[sdl::ShaderProgram] Failed to load and link, opengl program already generated");
		return false;
	}

	bool ShaderProgram::loadAndLink(const std::string& vShader, const std::string& fShader) {
		return loadAndLink(vShader, "", fShader);
	}

	void ShaderProgram::useProgram() const {
		if (programObjectId_ != 0) {
			glUseProgram(programObjectId_);
			assertGlError();
		} else {
			spdlog::warn("[sdl::ShaderProgram] Failed to use program, program is not compiled");
		}
	}
	
	bool ShaderProgram::linkProgram() {
		glLinkProgram(programObjectId_);
		GLint linked;
		glGetProgramiv(programObjectId_, GL_LINK_STATUS, &linked);
		if (linked == GL_FALSE) {
			logError<LogError::PROGRAM_ERROR>(programObjectId_, "", "Error linking program");
			glDeleteProgram(programObjectId_);
			return false;
		}
		return true;
	}
	

	void ShaderProgram::bindAllAttributes() {
		int index = 0;
		for (auto& [name, location] : attributes_) {
			glBindAttribLocation(programObjectId_, index, name.c_str());
			assertGlError();
			location = index++;
		}
	}

} // Namespace sdl.
