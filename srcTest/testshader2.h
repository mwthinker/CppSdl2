#ifndef TESTSHADER2_H
#define TESTSHADER2_H

#include "types.h"

#include <sdl/opengl.h>
#include <sdl/shaderprogram.h>

#include <string>

class TestShader2 {
public:
	TestShader2();
	TestShader2(std::string vShader, std::string fShader);

	void useProgram() const;
	void setVertexAttribPointer() const;

	// Uniforms. -------------------------------------------

	void setProjectionMatrix(const Mat44& matrix) const;
	void setModelMatrix(const Mat44& matrix) const;

	class Vertex {
	public:
		Vertex() = default;

		Vertex(GLfloat x, GLfloat y) : pos_(x, y), tex_(0, 0), color_(1, 1, 1, 1), texture_(0) {
		}

		Vertex(GLfloat x, GLfloat y, GLfloat xTex, GLfloat yTex) : pos_(x, y), tex_(xTex, yTex), color_(1, 1, 1, 1), texture_(1.f) {
		}

		Vec2 pos_;
		Vec2 tex_;
		Color color_;
		GLfloat texture_;
	};

private:
	sdl::ShaderProgram shader_;

	int aPosIndex_;
	int aTexIndex_;
	int aColorIndex_;
	int aTextureIndex_;

	int uProjIndex_;
	int uModelIndex_;
};

#endif // TESTSHADER2_H
