#include "testshader.h"
#include "types.h"

#include <sdl/window.h>

TestShader::TestShader(const std::string& vShader, const std::string& fShader) {
	shader_.bindAttribute("aPos");
	shader_.bindAttribute("aTex");
	shader_.bindAttribute("aColor");
	shader_.bindAttribute("aTexture");

	if (shader_.loadAndLinkFromFile(vShader, fShader)) {
		aPosIndex_ = shader_.getAttributeLocation("aPos");
		aTexIndex_ = shader_.getAttributeLocation("aTex");
		aColorIndex_ = shader_.getAttributeLocation("aColor");
		aTextureIndex_ = shader_.getAttributeLocation("aTexture");
		
		uProjIndex_ = shader_.getUniformLocation("uProj");
		uModelIndex_ = shader_.getUniformLocation("uModel");
	}
}

void TestShader::useProgram() {
	shader_.useProgram();
}

void TestShader::setVertexAttribPointer() const {
	size_t size = 0;

	gl::glEnableVertexAttribArray(aPosIndex_);
	gl::glVertexAttribPointer(aPosIndex_, 2, gl::GL_FLOAT, gl::GL_FALSE, sizeof(Vertex), (gl::GLvoid*) size);
	size += sizeof(Vertex::pos_);

	gl::glEnableVertexAttribArray(aTexIndex_);
	gl::glVertexAttribPointer(aTexIndex_, 2, gl::GL_FLOAT, gl::GL_FALSE, sizeof(Vertex), (gl::GLvoid*) size);
	size += sizeof(Vertex::tex_);

	gl::glEnableVertexAttribArray(aColorIndex_);
	gl::glVertexAttribPointer(aColorIndex_, 4, gl::GL_UNSIGNED_BYTE, gl::GL_TRUE, sizeof(Vertex), (gl::GLvoid*) size);
	size += sizeof(Vertex::color_);

	gl::glEnableVertexAttribArray(aTextureIndex_);
	gl::glVertexAttribPointer(aTextureIndex_, 1, gl::GL_FLOAT, gl::GL_FALSE, sizeof(Vertex), (gl::GLvoid*) size);
}

void TestShader::setProjectionMatrix(const Mat44& matrix) {
	shader_.useProgram();
	gl::glUniformMatrix4fv(uProjIndex_, 1, false, glm::value_ptr(matrix));
}

void TestShader::setModelMatrix(const Mat44& matrix) {
	shader_.useProgram();
	gl::glUniformMatrix4fv(uModelIndex_, 1, false, glm::value_ptr(matrix));
}
