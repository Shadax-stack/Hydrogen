#include "Camera.h"
#include <math.h>

namespace Hydrogen {

	//Maybe I will implement this as a lambda or put this in a math tool box file thingy
	inline static glm::vec3 CalculateDirection(const glm::vec3 & rotation) {
		return glm::vec3(
			cos(rotation.x) * cos(rotation.y),
			sin(rotation.y),
			sin(rotation.x) * cos(rotation.y)
		);
	}

	HydrogenCamera::HydrogenCamera(void) : Position(0.0f), Rotation([]()-> const float {return glm::radians(-90.0f); }(), 0.0f, 0.0f), AspectRatio([]() -> const float { return 16.0f / 9.0f; } ()), FieldOfView(45.0f), Near(0.1f), Far(100.0f) {
		Exposure = 1.0f;
		Contrast = 1.0f;
	}
	glm::mat4 HydrogenCamera::CreateIsometricMatrix(void) const {
		return glm::perspective(FieldOfView, AspectRatio, Near, Far);
	}

	glm::mat4 HydrogenCamera::CreateViewMatrix(void) const {
		return glm::lookAt(Position, Position + CalculateDirection(Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void HydrogenCamera::IncreasePosition(const glm::vec3 & increment) {
		Position += increment;
	}

	void HydrogenCamera::SetAspectRatio(const float& val) { 
		AspectRatio = val;
	}

	void HydrogenCamera::SetPosition(const glm::vec3& pos) {
		Position = pos;
	}

	glm::vec3 HydrogenCamera::GetPosition(void) const {
		return Position;
	}

	void HydrogenCamera::IncreaseRotation(const glm::vec3& increment) {
		Rotation += increment;
		Rotation.y = glm::clamp(Rotation.y, glm::radians(-89.99f), glm::radians(89.99f));
	}

	void HydrogenCamera::SetRotation(const glm::vec3& rot) {
		Rotation = rot;
	}

	glm::vec3 HydrogenCamera::GetRotation(void) const {
		return Rotation;
	}

	glm::vec3 HydrogenCamera::GetDirection(void) const {
		return CalculateDirection(Rotation);
	}

}