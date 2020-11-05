#ifndef HYDROGEN_MATERIAL_H
#define HYDROGEN_MATERIAL_H

#include "../Core/HydrogenEngine.h"
#include "Texture2D.h"
#include "Shader.h"
#include <map>

namespace Hydrogen {

	enum HydrogenPropertyType {
		HYDROGEN_MATERIAL_PROPERTY_FLOAT,
		HYDROGEN_MATERIAL_PROPERTY_VEC2,
		HYDROGEN_MATERIAL_PROPERTY_VEC3,
		HYDROGEN_MATERIAL_PROPERTY_TEXTURE_2D
	};

	enum HydrogenShadingMode {
		HYDROGEN_SHADING_MODE_FORWARD = true,
		HYDROGEN_SHADING_MODE_DEFERRED = false
	};

	struct HydrogenMaterialProperty {
		HydrogenPropertyType Type; //type: is it  a float, vec3, vec4, etc.
		int Int;
		float Float;
		HydrogenTexture2D* Texture2D;
		glm::vec2 Vec2;
		glm::vec3 Vec3;
	};

	//TODO: Add more to the material info, such as the diffuse/specular factors persented in the "materials" section of learnopengl.com/basic lighting
	struct HYDROGEN_API HydrogenMaterial {
		std::map<std::string, HydrogenMaterialProperty> Properties; //A need something way more fucking fast than a map for this material properties, maybe just an array/vector of pointers to a pair maybe
		HydrogenShader* Shader; //The shader we use to render the material 
		void SetShader(HydrogenShader* shader); //We don' need this since shader is public now but I'll just keep it incase shader becomes private again for some reason
		void SetProperty(const std::string& name, HydrogenTexture2D* val);
		void SetProperty(const std::string& name, glm::vec3 val);
		void SetProperty(const std::string& name, glm::vec2 val);
		void SetProperty(const std::string& name, float val);
		HydrogenMaterial(void);
		~HydrogenMaterial(void) = default;
	};

}

#endif
