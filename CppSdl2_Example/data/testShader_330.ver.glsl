#version 330 core
    
precision mediump float;

uniform mat4 uModel;
uniform mat4 uProj;

in vec4 aPos;
in vec2 aTex;
in vec4 aColor;
in float aTexture;

out vec2 vTex;
out vec4 vColor;
out float vTexture;

void main() {
	gl_Position = uProj * uModel * aPos;
	vTex = aTex;
	vColor = aColor;
	vTexture = aTexture;
}
