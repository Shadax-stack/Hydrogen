#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 2) in vec2 aTexCoord;

out vec2 fTexCoord;

void main() {
	gl_Position = vec4(aPosition, 1.0f);
	fTexCoord = aTexCoord;
}