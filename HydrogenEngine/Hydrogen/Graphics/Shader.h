#ifndef HYDROGEN_SHADER_H
#define HYDROGEN_SHADER_H

#include "../Core/HydrogenEngine.h"
#include "../Math/Camera.h"
#include "../Math/Transform.h"
#include "Texture2D.h"
#include "Lighting.h"
#include "Handle.h"
#include "Buffer.h"

namespace Hydrogen {
	//TODO: Deciede to use pointers or refrences for Upload*()
	class HYDROGEN_API HydrogenShader : public HydrogenHandle {
	private:
		int GetUniformLocation(const char* name) const;
		int GetUniformLocation(const std::string& name) const;
	public:
		HydrogenShader(void);
		~HydrogenShader(void);
		void CreateIdentifier(void);
		void DeleteIdentifier(void);
		void SetIdentifier(unsigned int id) = delete;
		void UseShader(void);
		void CompileShaderFiles(const char* vs, const char* fs);
		void CompileShaderSource(const char* vs, const char* fs);
		void UploadCamera(const char* name, const HydrogenCamera& camera) const;
		void UploadCamera(const std::string & name, const HydrogenCamera & camera) const;
		void UploadTransform(const char* name, const HydrogenTransform& transform) const;
		void UploadTransform(const std::string& name, const HydrogenTransform& transform) const;
		void UploadTexture(const char* name,  int binding) const;
		void UploadTexture(const std::string & name, int binding) const;
		void UploadDirectLight(const char* name, const HydrogenDirectLight& light) const;
		void UploadDirectLight(const std::string& name, const HydrogenDirectLight& light) const;
		void UploadPointLight(const char* name, const HydrogenPointLight& light) const;
		void UploadPointLight(const std::string& name, const HydrogenPointLight& light) const;
		void UploadFloat(const char* name, const float val) const;
		void UploadFloat(const std::string& name, const float val) const;
		void UploadVector3f(const char* name, const glm::vec3& vec) const;
		void UploadVector3f(const std::string& name, const glm::vec3& vec) const;
		void UploadInteger(const char* name, const int val) const;
		void UploadInteger(const std::string& name, const int val) const;
		void UploadMatrix4x4f(const char* name, const glm::mat4& val) const;
		void UploadMatrix4x4f(const std::string& name, const glm::mat4& val) const;
		void UploadMatrix3x3f(const char* name, const glm::mat3& val) const;
		void UploadMatrix3x3f(const std::string& name, const glm::mat3& val) const;
		void UploadMatrix2x2f(const char* name, const glm::mat2& val) const;
		void UploadMatrix2x2f(const std::string& name, const glm::mat2& val) const;
		void UploadBuffer(const char* name, HydrogenBuffer* buffer);
		void UploadBuffer(const std::string& name, HydrogenBuffer* buffer);
	};
}

#endif