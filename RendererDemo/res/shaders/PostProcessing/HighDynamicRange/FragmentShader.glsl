#version 330 core

in vec2 fTexCoord;

out vec3 color;

uniform sampler2D ColorBuffer;
uniform float Exposure;
uniform float Contrast;

void main() {
	color = texture(ColorBuffer, fTexCoord).rgb;
	color = 1.0f - exp(-Exposure * color);
	color = pow(color, vec3(Contrast));
} 