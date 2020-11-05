#include "Environment.h"

namespace Hydrogen {

	void HydrogenEnvironment::AddPointLight(HydrogenPointLight light) {
		AddPointLight(new HydrogenPointLight(light));
	}

	void HydrogenEnvironment::AddPointLight(HydrogenPointLight* light) {
		Lights.push_back(light);
	}

	std::vector<HydrogenPointLight*> HydrogenEnvironment::BuildPointLights(void) {
		std::vector<HydrogenPointLight*> PointLights;
		for (std::vector<HydrogenLight*>::iterator Light = Lights.begin(); Light != Lights.end(); Light++) {
			if ((*Light)->LightType == HYDROGEN_LIGHT_TYPE_POINT) {
				PointLights.push_back((HydrogenPointLight*)*Light);
			}
		}
		return PointLights;
	}

	HydrogenEnvironment::HydrogenEnvironment(void) {
		
	}

}