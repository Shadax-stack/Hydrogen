#version 330 core

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;
in vec3 fTangent;
in vec3 fBitangent;

out vec4 color;

uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;
uniform sampler2D HeightMap;
uniform sampler2D MetallicMap;
uniform sampler2D RoughnessMap;
uniform sampler2D AmbientOcclusionMap;

struct Light {
	vec3 Luminosity;
	vec3 Position;
};

uniform struct {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 Position;
} Camera;

layout(std140) uniform LightBuffer{
	Light Lights[64];
	int NumLights;
};

const float MATH_PI = 3.14159265359;

struct MaterialContext {
	vec3 F0;
	float roughness;
	float alpha;
	float alpha_2;
	float metalness;
	vec3 normal;
	vec3 light;
	vec3 view;
	vec3 halfway;
	float NoL;
	float NoV;
	float NoH;
	float HoV;
	float HoL;
} CurrentContext;;

float GeometricShadowingShlickGGX(vec3 v, vec3 n, float alpha_2) {
	float NoV = max(dot(n, v), 0.0f);
	return (2 * NoV) / (NoV + sqrt(alpha_2 + (1 - alpha_2) * (NoV * NoV)));
}

float GeometricShadowingSmith(vec3 l, vec3 v, vec3 n, float roughness) {
	float alpha_2 = roughness * roughness;
	return GeometricShadowingShlickGGX(l, n, alpha_2) * GeometricShadowingShlickGGX(v, n, alpha_2);
}

vec3 FresnelSchlick(vec3 F0, float cosTheta) {
	return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}

float NormalDistributionFunctionTrowbridgeReitzGGX(vec3 n, vec3 h, float roughness) {
	float NoH = max(dot(n, h), 0.0f);
	float k = roughness / ((NoH * NoH) * (roughness * roughness - 1) + 1);
	return k * k / MATH_PI;
}

float VisibilitySmithGGX(vec3 n, vec3 v, vec3 l, float roughness) {
	float alpha_2 = roughness * roughness;
	float smoothness_alpha_2 = 1.0f - alpha_2;
	float NoV = max(dot(n, v), 0.0f);
	float NoL = max(dot(n, l), 0.0f);
	float GGX_V = NoL * sqrt(NoV * NoV * smoothness_alpha_2 + alpha_2);
	float GGX_L = NoV * sqrt(NoL * NoL * smoothness_alpha_2 + alpha_2);
	return 0.5 / (GGX_V + GGX_L);
}

vec3 SpecularCookTorrance(vec3 view, vec3 light, vec3 normal, vec3 diffuse, float metalness, float roughness) {
	roughness *= roughness;
	vec3 halfway = normalize(view + light);
	float NoL = max(dot(normal, light), 0.0f);
	float NoV = max(dot(normal , view), 0.0f);
	float HoV = max(dot(halfway, view), 0.0f);
	float HoL = max(dot(halfway, light), 0.0f);
	float HoN = max(dot(halfway, normal), 0.0f);
	vec3 F0 = mix(vec3(0.04f), diffuse, metalness);
	return NormalDistributionFunctionTrowbridgeReitzGGX(normal, halfway, roughness) * FresnelSchlick(F0, HoN) * VisibilitySmithGGX(normal, view, light, roughness);
}

float FresnelSchlick(float mu, float f0, float f90) {
	return f0 + (f90 - f0) * pow(1.0f - mu, 5.0f);
}

//Taken from https://docs.google.com/document/d/1ZLT1-fIek2JkErN9ZPByeac02nWipMbO89oCW2jxzXo/edit
vec3 DiffuseDevshLambert(vec3 v, vec3 l, vec3 n, vec3 d, float metalness) {
	vec3 Lambert = (1.0f - metalness) * d / MATH_PI;
	float NoV = max(dot(n, v), 0.0f);
	float NoL = max(dot(n, l), 0.0f);
	vec3 F0 = mix(vec3(0.04), d, metalness);
	return (1.0f - FresnelSchlick(F0, NoV)) * (1.0f - FresnelSchlick(F0, NoL)) * Lambert;
}

//Cook-Torrance and Devsh-Lambert
vec3 BidirectionalReflectanceDistributionFunction(vec3 omega_o, vec3 omega_i, vec3 normal, vec3 diffuse, in float metalness, float roughness) {
	return SpecularCookTorrance(omega_o, omega_i, normal, diffuse, metalness, roughness) + DiffuseDevshLambert(omega_o, omega_i, normal, diffuse, metalness);
}
 
const int NumSteps = 16;
const float LayerDepth = 1.0f / float(NumSteps);
const float ParallaxScale = 0.5f;

//Broken for a reason I do not know
vec2 ParallaxOcclusionMapping(vec3 v) {
	float CurrentLayerDepth = 0.0f;
	vec2 P = v.xy / v.z * ParallaxScale;
	vec2 LayerDecrement = P / float(NumSteps);
	vec2 CurrentTexCoords = fTexCoord;
	float CurrentDepth = texture(HeightMap, CurrentTexCoords).r;
	while (CurrentLayerDepth < CurrentDepth) {
		CurrentTexCoords -= LayerDecrement;
		CurrentDepth = texture(HeightMap, CurrentTexCoords).r;
		CurrentLayerDepth += LayerDepth;
	}
	return CurrentTexCoords;
}

void main() {
	mat3 TBN = mat3(normalize(fTangent), normalize(fBitangent), normalize(fNormal));
	mat3 TBN_transpose = transpose(TBN);
	vec3 viewDir = normalize(Camera.Position - fPosition);
	vec2 texCoords = fTexCoord;// ParallaxOcclusionMapping(normalize(TBN_transpose * viewDir));
	vec4 texColor = texture(DiffuseMap, texCoords);
	vec3 diffuse = texColor.rgb;
	vec3 normal = normalize(TBN * (texture(NormalMap, texCoords).rgb * 2.0f - 1.0f));
	float metallic = texture(MetallicMap, texCoords).r;
	float roughness = texture(RoughnessMap, texCoords).r;
	float ao = texture(AmbientOcclusionMap, texCoords).r;
	for (int index = 0; index < NumLights; index++) {
		Light CurrentLight = Lights[index];
		vec3 lightDir = normalize(CurrentLight.Position - fPosition);
		float attenuation = distance(fPosition, CurrentLight.Position);
		vec3 Radiance = ao * attenuation * CurrentLight.Luminosity * max(dot(normal, lightDir), 0.0f);
		color.rgb += BidirectionalReflectanceDistributionFunction(viewDir, lightDir, normal, diffuse, metallic, roughness) * Radiance;
	}
	color.a = texColor.a;
}