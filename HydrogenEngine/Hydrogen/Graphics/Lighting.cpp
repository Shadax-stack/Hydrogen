#include "Lighting.h"

namespace Hydrogen {

	HydrogenDirectLight::HydrogenDirectLight(void) : Direction(0.0f), Color(1.0f) {
		LightType = HYDROGEN_LIGHT_TYPE_DIRECT;
	}

	HydrogenPointLight::HydrogenPointLight(void) : Position(1.0f), Luminosity(1.0f) {
		LightType = HYDROGEN_LIGHT_TYPE_POINT;
	}

	HydrogenLight::HydrogenLight(void) : ShadowMode(HYDROGEN_SHADOWS_DISABLED) {

	}

}