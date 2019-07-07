#include "shaderprogram.h"
#include "opengl.h"
#include "window.h"
#include "logger.h"

#include <fstream>
#include <sstream>

namespace sdl {

	int ShaderProgram::currentProgramId = 0;

	namespace {		

		GLuint loadShader(GLuint program, GLenum type, const GLchar* shaderSrc) {
			GLuint shader= glCreateShader(type);
			glShaderSource(shader, 1, &shaderSrc, 0);
			checkGlError();
			glCompileShader(shader);
			checkGlError();
			glAttachShader(program, shader);
			checkGlError();
#if _DEBUG
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen > 1) {
				char message[256]; // A arbitrary value big enough to contain message.
				GLsizei size;
				glGetShaderInfoLog(shader, sizeof(message), &size, message);
				std::string str;
				str.append(message, message + size);
				logger()->error("[Shader] {} , error loading shader: {}", shaderSrc, str);
			}
#endif // _DEBUG
			return shader;
		}

		std::string loadFromFile(const std::string& file) {
			if (file.empty()) {
				return "";
			}
			std::ifstream inFile(file);
			std::stringstream stream;
			stream << inFile.rdbuf();
			return stream.str();
		}

	}

	ShaderProgram::ShaderProgram() :
		location_(0),
		programObjectId_(0),
		windowInstance_(0) {

	}

	ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept :
		attributes_(std::move(other.attributes_)),
		uniforms_(std::move(other.uniforms_)),
		location_(other.location_),
		programObjectId_(other.programObjectId_),
		windowInstance_(other.windowInstance_)
	{
		other.location_ = -1;
		other.programObjectId_ = -1;
		other.windowInstance_ = -1;
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
		attributes_ = std::move(other.attributes_);
		uniforms_ = std::move(other.uniforms_);
		location_ = other.location_;
		programObjectId_ = other.programObjectId_;
		windowInstance_ = other.windowInstance_;

		other.location_ = -1;
		other.programObjectId_ = -1;
		other.windowInstance_ = -1;
		return *this;
	}

	ShaderProgram::~ShaderProgram() {
		// Opengl program loaded? And the opengl context active?
		if (programObjectId_ != 0) {
			// Is called if the program is valid and therefore need to be cleaned up.
			glDeleteProgram(programObjectId_);
			checkGlError();
			currentProgramId = 0;
		}
	}

	void ShaderProgram::bindAttribute(const std::string& attribute) {
		if (programObjectId_ == 0) {
			attributes_[attribute] = location_;
		}
	}

	int ShaderProgram::getAttributeLocation(const std::string& attribute) const {
		auto it = attributes_.find(attribute);
		if (it != attributes_.end()) {
			return it->second;
		}
		return -1;
	}

	int ShaderProgram::getUniformLocation(const std::string& uniform) const {
		if (programObjectId_ != 0) {
			auto it = uniforms_.find(uniform);

			if (it != uniforms_.end()) {
				return it->second;
			} else {
				int loc = glGetUniformLocation(programObjectId_, uniform.c_str());
				checkGlError();
				if (loc != -1) {
					uniforms_[uniform] = loc;
				}
				return loc;
			}
		}
		return -1;
	}

	bool ShaderProgram::loadAndLinkFromFile(const std::string& vShaderFile, const std::string& gShaderFile, const std::string& fShaderFile) {
		if (programObjectId_ == 0) {
			return loadAndLink(loadFromFile(vShaderFile), loadFromFile(gShaderFile), loadFromFile(fShaderFile));
		}
		return false;
	}

	bool ShaderProgram::loadAndLinkFromFile(const std::string& vShaderFile, const std::string& fShaderFile) {
		return loadAndLinkFromFile(vShaderFile, "", fShaderFile);
	}

	bool ShaderProgram::loadAndLink(const std::string& vShader, const std::string& gShader, const std::string& fShader) {
		if (programObjectId_ == 0) {
			programObjectId_ = glCreateProgram();
			checkGlError();
			
			if (programObjectId_ == 0) {
				return false;
			}

			loadShader(programObjectId_, GL_VERTEX_SHADER, vShader.c_str());
			if (!gShader.empty()) {
				loadShader(programObjectId_, GL_GEOMETRY_SHADER, gShader.c_str());
			}
			loadShader(programObjectId_, GL_FRAGMENT_SHADER, fShader.c_str());

			// Bind all attributes.
			for (auto& pair : attributes_) {
				glBindAttribLocation(programObjectId_, location_, pair.first.c_str());
				checkGlError();
				pair.second = location_++;
			}

			glLinkProgram(programObjectId_);

			GLint linked;
			glGetProgramiv(programObjectId_, GL_LINK_STATUS, &linked);
			if (!linked) {
				GLint infoLen = 0;
				glGetProgramiv(programObjectId_, GL_INFO_LOG_LENGTH, &infoLen);
				if (infoLen > 1) {
					char message[256];
					GLsizei size;
					glGetProgramInfoLog(programObjectId_, sizeof(message), &size, message);
					std::string str;
					str.append(message, message + size);
					logger()->error("[Shader] Error linking program: {}", str);
				}
				glDeleteProgram(programObjectId_);
				return false;
			}

			useProgram();
			return true;
		}
		return false;
	}

	bool ShaderProgram::loadAndLink(const std::string& vShader, const std::string& fShader) {
		return loadAndLink(vShader, "", fShader);
	}

	void ShaderProgram::useProgram() const {
		if (programObjectId_ != 0 && currentProgramId != programObjectId_) {
			glUseProgram(programObjectId_);
			checkGlError();
			currentProgramId = programObjectId_;
		}
	}

} // Namespace sdl.
