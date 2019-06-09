#version 120
    
precision mediump float;

uniform mat4 uModel;
uniform mat4 uProj;
uniform float uIsTexture;

attribute vec4 aPos;
attribute vec2 aTex;

varying vec2 vTexCoord;

void main() {
	gl_Position = uProj * uModel * aPos;
	if (uIsTexture > 0.5) {
		vTexCoord = aTex;
	}
}
