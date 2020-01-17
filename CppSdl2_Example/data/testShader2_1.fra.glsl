#version 120

precision mediump float;

uniform sampler2D uTexture;
uniform vec4 uColor;
uniform float uIsTexture;

varying vec2 vTexCoord;

void main() {
	if (uIsTexture < 0.5) {
		gl_FragColor = uColor;
	} else {
		vec4 tex = texture2D(uTexture, vTexCoord); 
		gl_FragColor = vec4(uColor.x * tex.x, uColor.y * tex.y, uColor.z * tex.z, uColor.a * tex.a);
	}
}
