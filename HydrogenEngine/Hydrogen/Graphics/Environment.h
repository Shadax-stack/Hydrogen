#ifndef HYDROGEN_ENVIRONMENT_H
#define HYDROGEN_ENVIRONMENT_H

#include "../Core/HydrogenEngine.h"
#include "Lighting.h"
#include "Buffer.h"
#include <vector>

namespace Hydrogen {

	//Returning to my code like a month later using an inhertiance based approach to lights for my light vector was kinda autistic

	//Contains all the stuff for the environment, such as lights, irrandiance probes, skyboxes, etc. Each scene has it's own enviornment. We use pointers to reduce redundant resources such as skyboxes
	class HYDROGEN_API HydrogenEnvironment {
	public:
		HydrogenBuffer LightBuffer; //entirely managed by renderer
		std::vector<HydrogenLight*> Lights; //I just use a generic light pointer to just store it all in a single data structure
		void AddPointLight(HydrogenPointLight light);
		void AddPointLight(HydrogenPointLight* light);
		//BuildXLights creates a vector containing those type of lights
		std::vector<HydrogenPointLight*> BuildPointLights(void);
		HydrogenEnvironment(void);
	};

}

#endif