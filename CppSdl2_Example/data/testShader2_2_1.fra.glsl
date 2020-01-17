#version 120

precision mediump float;

uniform sampler2D uTexture;

varying vec2 vTex;
varying vec4 vColor;
varying float vTexture;

void main() {
	if (vTexture > 0.5) {
		vec4 c = texture2D(uTexture, vTex); 
		gl_FragColor = vec4(vColor.r * c.r, vColor.g * c.g, vColor.b * c.b, vColor.a * c.a);
	} else {
		gl_FragColor = vColor;
	}
}
