#ifndef HYDROGEN_TRANSFORM_H
#define HYDROGEN_TRANSFORM_H

#include "../Core/HydrogenEngine.h"
#include "LinearAlgebra.h"

namespace Hydrogen {

	class HYDROGEN_API HydrogenTransform {
	private:
		glm::vec3 Translation;
		glm::vec3 Rotation;
		/*
		TODO:
		Fix the rotation so it works with the camera
		Like:
		x - yaw
		y - pitch
		z - roll
		*/
		glm::vec3 Scale;
		//TODO: Update this terrible variable name
		glm::vec2 UVCoordMult;
	public:
		HydrogenTransform(void);
		~HydrogenTransform(void) = default;
		glm::mat4 CreateModelMatrix(void) const;
		glm::mat3 CreateNormalMatrix(void) const;
		glm::mat2 CreateTextureMatrix(void) const;
		void IncreaseScale(const glm::vec3& val);
		void IncreaseRotation(const glm::vec3& val);
		void IncreaseTranslation(const glm::vec3& val);
		void SetScale(const glm::vec3& val);
		void SetRotation(const glm::vec3& val);
		void SetTranslation(const glm::vec3& val);
		glm::vec3 GetScale(void) const;
		glm::vec3 GetRotation(void) const;
		glm::vec3 GetTranslation(void) const;

		void IncreaseUVCoordMult(const glm::vec2& val);
		void SetUVCoordMult(const glm::vec2& val);
		glm::vec2 GetUVCoordMult(void) const;


	};

}

#endif