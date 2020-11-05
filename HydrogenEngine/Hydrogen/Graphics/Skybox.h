#ifndef HYDROGEN_SKYBOX_H
#define HYDROGEN_SKYBOX_H

#include "../Core/HydrogenEngine.h"
#include "TextureCubemap.h"
#include "Component.h"

namespace Hydrogen {

	struct HYDROGEN_API HydrogenSkyboxRenderingComponents : public HydrogenComponents {
		HydrogenTextureCubemap Skybox;
	};

	class HYDROGEN_API HydrogenSkybox {
	protected:
		HydrogenSkyboxRenderingComponents Components;
	public:
		HydrogenSkybox(void);
		~HydrogenSkybox(void);
		void LoadSkybox(const std::string& path);
		void ActivateTextureUnits(void);
	};

}

#endif