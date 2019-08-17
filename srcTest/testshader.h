#ifndef TESTSHADER_H
#define TESTSHADER_H

#include <sdl/opengl.h>
#include <sdl/shaderprogram.h>

#include "types.h"

#include <string>

class TestShader {
public:
	TestShader() = default;
    TestShader(const std::string& vShader, const std::string& fShader);

	static constexpr unsigned int vertexSizeInBytes() {
		return vertexSizeInFloat() * sizeof(GLfloat);
	}

	static constexpr unsigned int vertexSizeInFloat() {
		return 4;
	}

	void useProgram() const;
    void setVertexAttribPointer() const;

    // Uniforms. -------------------------------------------

    void setProjectionMatrixU(const Mat44& matrix) const;
    void setModelMatrixU(const Mat44& matrix) const;

    void setColorU(float red, float green, float blue, float alpha = 1) const;
    void setColorU(const Color& color) const;
    void setTextureU(bool texture) const;

private:
    sdl::ShaderProgram shaderProgram_;

    int aPosIndex_ = -1;
    int aTexIndex_ = -1;

    int uProjIndex_ = -1;
    int uModelIndex_ = -1;
    
	int uColorIndex_ = -1;
    int uIsTexIndex_ = -1;
};

#endif // TESTSHADER_H
