#version 330 core

in vec3 fTexCoord;

out vec3 color;

uniform samplerCube Skybox;

void main() {
	color = texture(Skybox, fTexCoord).rgb;
}