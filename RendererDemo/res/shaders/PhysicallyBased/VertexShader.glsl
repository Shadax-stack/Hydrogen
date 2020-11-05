/*
Cook Torrance BRDF vertex shader
*/
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexCoord;
out vec3 fTangent;
out vec3 fBitangent;

uniform struct {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 Position;
} Camera;

uniform struct {
	mat2 TextureMatrix;
	mat3 NormalMatrix;
	mat4 ModelMatrix;
} Transform;

void main() {
	vec4 worldPos = Transform.ModelMatrix * vec4(aPosition, 1.0f);
	gl_Position = Camera.ProjectionMatrix * Camera.ViewMatrix * worldPos;
	fPosition = worldPos.xyz;
	fTexCoord = Transform.TextureMatrix * aTexCoord;
	fNormal = Transform.NormalMatrix * aNormal;
	fTangent = Transform.NormalMatrix * aTangent;
	fBitangent = Transform.NormalMatrix * aBitangent;
}
