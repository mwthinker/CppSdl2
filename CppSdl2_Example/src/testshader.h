#ifndef TESTSHADER_H
#define TESTSHADER_H

#include "types.h"

#include <sdl/color.h>
#include <sdl/opengl.h>
#include <sdl/shaderprogram.h>

#include <string>

class TestShader {
public:
	TestShader() = default;
	TestShader(const std::string& vShader, const std::string& fShader);

	void useProgram();
	void setVertexAttribPointer() const;

	void setProjectionMatrix(const Mat44& matrix);
	void setModelMatrix(const Mat44& matrix);

	class Vertex {
	public:
		Vertex() = default;

		Vertex(gl::GLfloat x, gl::GLfloat y)
			: pos_{x, y} {
		}

		Vertex(gl::GLfloat x, gl::GLfloat y, gl::GLfloat xTex, gl::GLfloat yTex)
			: pos_{x, y}
			, tex_{xTex, yTex}
			, texture_{1.f} {
		}

		Vec2 pos_{};
		Vec2 tex_{};
		sdl::Color color_{sdl::color::White};
		gl::GLfloat texture_{};
	};

private:
	sdl::ShaderProgram shader_;

	int aPosIndex_ = -1;
	int aTexIndex_ = -1;
	int aColorIndex_ = -1;
	int aTextureIndex_ = -1;

	int uProjIndex_ = -1;
	int uModelIndex_ = -1;
};

#endif
