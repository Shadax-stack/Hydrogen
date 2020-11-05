#ifndef HYDROGEN_LIGHTING_H
#define HYDROGEN_LIGHTING_H

#include "../Core/HydrogenEngine.h"

//My plan for the light managment system is to create a vector to manages pointers to lightes, and use RegisterLight to function. Though I really want a lighting system like direct3d 9's

namespace Hydrogen {

	enum HydrogenLightType {
		HYDROGEN_LIGHT_TYPE_POINT,
		HYDROGEN_LIGHT_TYPE_DIRECT,
	};

	enum HydrogenShadowsMode {
		HYDROGEN_SHADOWS_DISABLED = false,
		HYDROGEN_SHADOWS_ENABLED_DIRECT,
		HYDROGEN_SHADOWS_ENABLED_CUBE_MAP,
		HYDROGEN_SHADOWS_ENABLED_EQUIRECTANGULAR, //I seem to like EQUIRECTANGULAR projection instead because we don't have to render the scene 6 times or use a geometry shader
	};

	struct HYDROGEN_API HydrogenLight {
		HydrogenLightType LightType; //What type of light is this? (we will fill this out in the substruct constructor)
		HydrogenShadowsMode ShadowMode; //Do we need to render a shadow map for this?
		HydrogenLight(void);
		~HydrogenLight(void) = default;
	};

	//TODO: Possible name is shader: HydrogenShaderDirectLight,ShaderDirectLight, requires change to naming convention, will do later
	struct HYDROGEN_API HydrogenDirectLight : public HydrogenLight {
		glm::vec3 Direction;
		glm::vec3 Color;
		HydrogenDirectLight(void);
		~HydrogenDirectLight(void) = default;
	};

	struct HYDROGEN_API HydrogenPointLight : public HydrogenLight {
		glm::vec3 Luminosity;
		glm::vec3 Position;
		HydrogenPointLight(void);
		~HydrogenPointLight(void) = default;
	};

	/*
	Two equations for cheap spherical area lights (taken from http://www.paulsprojects.net/opengl/ppatten/ppatten.html):
	1. A = 1 - (d^2) / (r^2)
	2. A = exp(-k * d^2)
	Variables:
	A - Attenuation
	d - distance
	r - radius of light
	k - coefficent idk

	UPDATE: the equations were incorrect

	*/

	//TODO: Add spot light

}

#endif