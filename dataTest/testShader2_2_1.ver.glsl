#version 120
    
precision mediump float;

uniform mat4 uModel;
uniform mat4 uProj;

attribute vec4 aPos;
attribute vec2 aTex;
attribute vec4 aColor;
attribute float aTexture;

varying vec2 vTex;
varying vec4 vColor;
varying float vTexture;

void main() {
	gl_Position = uProj * uModel * aPos;
	vTex = aTex;
	vColor = aColor;
	vTexture = aTexture;
}
