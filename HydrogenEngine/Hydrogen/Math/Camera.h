#ifndef HYDROGEN_CAMERA_H
#define HYDROGEN_CAMERA_H

#include "../Core/HydrogenEngine.h"
#include "Transform.h"

namespace Hydrogen {

	//Call RegisterCamera() in renderer so the FOV and AspectRatio can automaticialy update on window resize
	class HYDROGEN_API HydrogenCamera {
	private:
		glm::vec3 Position;
		glm::vec3 Rotation;
		/*
		x - Yaw
		y - Pitch
		z - Roll (not yet implemented!)
		*/
		float AspectRatio;
		float FieldOfView;
		float Near;
		float Far;
	public:
		float Contrast;
		float Exposure;
		HydrogenCamera(void); //By default the camera is pointing in the -z direction
		~HydrogenCamera(void) = default;
		glm::mat4 CreateIsometricMatrix(void) const;
		glm::mat4 CreateViewMatrix(void) const;
		void IncreasePosition(const glm::vec3 & increment);
		void SetPosition(const glm::vec3& pos);
		glm::vec3 GetPosition(void) const;
		void IncreaseRotation(const glm::vec3& increment);
		void SetRotation(const glm::vec3& rot);
		glm::vec3 GetRotation(void) const;
		glm::vec3 GetDirection(void) const;
		void SetAspectRatio(const float& val);
	};

}

#endif
