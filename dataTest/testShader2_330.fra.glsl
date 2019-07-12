#version 330 core

precision mediump float;

uniform sampler2D uTexture;

in vec2 vTex;
in vec4 vColor;
in float vTexture;

out vec4 OutColor;

void main() {
	if (vTexture > 0.5) {
		vec4 c = texture2D(uTexture, vTex);
		OutColor = vec4(vColor.r * c.r, vColor.g * c.g, vColor.b * c.b, vColor.a * c.a);
	} else {
		OutColor = vColor;
	}
}
