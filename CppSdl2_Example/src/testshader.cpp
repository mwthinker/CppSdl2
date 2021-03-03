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

void TestShader::useProgram() const {
	shader_.useProgram();
}

void TestShader::setVertexAttribPointer() const {
	size_t size = 0;

	glEnableVertexAttribArray(aPosIndex_);
	glVertexAttribPointer(aPosIndex_, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) size);
	size += sizeof(Vertex::pos_);

	glEnableVertexAttribArray(aTexIndex_);
	glVertexAttribPointer(aTexIndex_, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) size);
	size += sizeof(Vertex::tex_);

	glEnableVertexAttribArray(aColorIndex_);
	glVertexAttribPointer(aColorIndex_, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (GLvoid*) size);
	size += sizeof(Vertex::color_);

	glEnableVertexAttribArray(aTextureIndex_);
	glVertexAttribPointer(aTextureIndex_, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) size);
	sdl::assertGlError();
}

// Uniforms. -------------------------------------------

void TestShader::setProjectionMatrix(const Mat44& matrix) const {
	shader_.useProgram();
	glUniformMatrix4fv(uProjIndex_, 1, false, glm::value_ptr(matrix));
}

void TestShader::setModelMatrix(const Mat44& matrix) const {
	shader_.useProgram();
	glUniformMatrix4fv(uModelIndex_, 1, false, glm::value_ptr(matrix));
}
