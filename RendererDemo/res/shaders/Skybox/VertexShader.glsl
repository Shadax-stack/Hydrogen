#version 330 core

layout(location = 0) in vec3 aPosition;

out vec3 fTexCoord;

uniform struct {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 Position;
} Camera;

void main() {
	gl_Position = Camera.ProjectionMatrix * vec4(mat3(Camera.ViewMatrix) * aPosition, 1.0f);
	fTexCoord = aPosition;
}