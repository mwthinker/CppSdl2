#include "testshader.h"
#include "types.h"

TestShader::TestShader() {
    aPosIndex_ = -1;
    aTexIndex_ = -1;

    // Collect the vertex buffer uniforms indexes.
    uProjIndex_ = -1;
    uModelIndex_ = -1;

    // Collect the fragment buffer uniforms indexes.
    uColorIndex_ = -1;
    uIsTexIndex_ = -1;
}

TestShader::TestShader(const std::string& vShader, const std::string& fShader) {
	shaderProgram_.bindAttribute("aPos");
	shaderProgram_.bindAttribute("aTex");
	shaderProgram_.loadAndLinkFromFile(vShader, fShader);

	shaderProgram_.useProgram();

    // Collect the vertex buffer attributes indexes.
    aPosIndex_ = shaderProgram_.getAttributeLocation("aPos");
    aTexIndex_ = shaderProgram_.getAttributeLocation("aTex");

    // Collect the vertex buffer uniforms indexes.
    uProjIndex_ = shaderProgram_.getUniformLocation("uProj");
    uModelIndex_ = shaderProgram_.getUniformLocation("uModel");

    // Collect the fragment buffer uniforms indexes.
    uColorIndex_ = shaderProgram_.getUniformLocation("uColor");
    uIsTexIndex_ = shaderProgram_.getUniformLocation("uIsTexture");
}

void TestShader::useProgram() const {
	shaderProgram_.useProgram();
}

void TestShader::setVertexAttribPointer() const {
	glEnableVertexAttribArray(aPosIndex_);
	glVertexAttribPointer(aPosIndex_, 2, GL_FLOAT, GL_FALSE, vertexSizeInBytes(), (GLvoid*) (sizeof(GLfloat) * 0));
	glEnableVertexAttribArray(aTexIndex_);
	glVertexAttribPointer(aTexIndex_, 2, GL_FLOAT, GL_FALSE, vertexSizeInBytes(), (GLvoid*) (sizeof(GLfloat) * 2));
	sdl::checkGlError();
}

// Uniforms. -------------------------------------------

void TestShader::setProjectionMatrixU(const Mat44& matrix) const {
	shaderProgram_.useProgram();
    glUniformMatrix4fv(uProjIndex_, 1, false, glm::value_ptr(matrix));
}

void TestShader::setModelMatrixU(const Mat44& matrix) const {
	shaderProgram_.useProgram();
    glUniformMatrix4fv(uModelIndex_, 1, false, glm::value_ptr(matrix));
}

void TestShader::setColorU(float red, float green, float blue, float alpha) const {
	shaderProgram_.useProgram();
    glUniform4f(uColorIndex_, red, green, blue, alpha);
}

void TestShader::setColorU(const Color& color) const {
	shaderProgram_.useProgram();
    glUniform4f(uColorIndex_, color.r, color.g, color.b, color.a);
}

void TestShader::setTextureU(bool texture) const {
	shaderProgram_.useProgram();
    if (texture) {
        glUniform1f(uIsTexIndex_, 1);
    } else {
        glUniform1f(uIsTexIndex_, 0);
    }
}
