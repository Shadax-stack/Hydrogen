#include "Transform.h"

namespace Hydrogen {

	HydrogenTransform::HydrogenTransform(void) : Scale(1.0f), Rotation(0.0f), Translation(0.0f), UVCoordMult(1.0f) {}

	glm::mat4 HydrogenTransform::CreateModelMatrix(void) const {
		glm::mat4 transform(1.0f);
		transform = glm::scale(transform, Scale);
		//Not conforming to the XYZ so it's YAW_PITCH_ROLL instead
		transform = glm::rotate(transform, Rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::rotate(transform, Rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		transform[3] = glm::vec4(Translation, 1.0f);
		return transform;
	}

	void HydrogenTransform::IncreaseScale(const glm::vec3& val) {
		Scale += val;
	}

	void HydrogenTransform::IncreaseRotation(const glm::vec3& val) {
		Rotation += val;
	}

	void HydrogenTransform::IncreaseTranslation(const glm::vec3& val) {
		Translation += val;
	}

	void HydrogenTransform::SetScale(const glm::vec3& val) {
		Scale = val;
	}

	void HydrogenTransform::SetRotation(const glm::vec3& val) {
		Rotation = val;
	}

	void HydrogenTransform::SetTranslation(const glm::vec3& val) {
		Translation = val;
	}

	glm::vec3 HydrogenTransform::GetScale(void) const {
		return Scale;
	}

	glm::vec3 HydrogenTransform::GetRotation(void) const {
		return Rotation;
	}

	glm::vec3 HydrogenTransform::GetTranslation(void) const {
		return Translation;
	}

	void HydrogenTransform::IncreaseUVCoordMult(const glm::vec2& val) {
		UVCoordMult += val;
	}

	void HydrogenTransform::SetUVCoordMult(const glm::vec2& val) {
		UVCoordMult = val;
	}

	glm::vec2 HydrogenTransform::GetUVCoordMult(void) const {
		return UVCoordMult;
	}

	glm::mat2 HydrogenTransform::CreateTextureMatrix(void) const {
		glm::mat2 TextureMatrix;
		TextureMatrix[0] = glm::vec2(UVCoordMult.x, 0.0f);
		TextureMatrix[1] = glm::vec2(0.0f, UVCoordMult.y);
		return TextureMatrix;
		//return glm::mat2(10.0f, 0.0f, 0.0f, 10.0f);
	}

	glm::mat3  HydrogenTransform::CreateNormalMatrix(void) const {
		return  glm::mat3(glm::inverse(glm::transpose(CreateModelMatrix())));
	}

}