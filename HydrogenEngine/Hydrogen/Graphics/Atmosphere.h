#ifndef HYDROGEN_ATMOSPHERE_H
#define HYDROGEN_ATMOSPHERE_H

#include "../Core/HydrogenEngine.h"
#include "Skybox.h"
#include "Framebuffer.h"

namespace Hydrogen {

	struct HydrogenAtmosphereProperties {
		float AtmosphereRadius;
		float PlanetRadius;
		float ScaleHeightRayleigh;
		float ScaleHeightMie;

	};

	class HydrogenAtmosphere : public HydrogenSkybox {
	private:
		HydrogenFramebuffer AtmosphereFramebuffer;
		HydrogenAtmosphereProperties AtmosphereProperties;
	};

}

#endif 
