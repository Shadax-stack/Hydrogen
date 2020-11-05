#include "Material.h"

namespace Hydrogen {

	HydrogenMaterial::HydrogenMaterial(void) : Shader(nullptr) {

	}

	void HydrogenMaterial::SetShader(HydrogenShader* shader) {
		Shader = shader;
	}

	void HydrogenMaterial::SetProperty(const std::string& name, glm::vec3 val) {
		HydrogenMaterialProperty* property = &Properties[name];
		property->Type = HYDROGEN_MATERIAL_PROPERTY_VEC3;
		property->Vec3 = val;
	}

	void HydrogenMaterial::SetProperty(const std::string& name, glm::vec2 val) {
		HydrogenMaterialProperty* property = &Properties[name];
		property->Type = HYDROGEN_MATERIAL_PROPERTY_VEC3;
		property->Vec2 = val;
	}

	void HydrogenMaterial::SetProperty(const std::string& name, float val) {
		HydrogenMaterialProperty* property = &Properties[name];
		property->Type = HYDROGEN_MATERIAL_PROPERTY_VEC3;
		property->Float = val;
	}

	void HydrogenMaterial::SetProperty(const std::string& name, HydrogenTexture2D* val) {
		//It doesn't matter what value we assign to the texture as long as we activate it with the same number
		//We just use a counter that starts at 0 when uniforming the values, and we post-increment it when we see a propety with the texture tag
		HydrogenMaterialProperty* property = &Properties[name];
		property->Type = HYDROGEN_MATERIAL_PROPERTY_TEXTURE_2D;
		property->Texture2D = val;
	}

}