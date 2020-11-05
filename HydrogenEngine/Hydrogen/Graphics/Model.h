#ifndef HYDROGEN_MODEL_H
#define HYDROGEN_MODEL_H

#include "../Core/HydrogenEngine.h"
#include "Mesh.h"
#include "Texture2D.h"
#include "../Math/Transform.h"
#include "Component.h"



namespace Hydrogen {
		
	enum HydrogenBlendingInfo {
		HYDROGEN_BLENDING_INFO_OPAQUE,
		HYDROGEN_BLENDING_INFO_TRANSPARENT,
		HYDROGEN_BLENDING_INFO_TRANSLUCENT,
	};

	//Basically this contains everything needed for rendering. Sutff like like physics stuff will be done sepreatly 
	struct HYDROGEN_API HydrogenModelRenderingComponents : public HydrogenComponents {
		struct {
			HydrogenTexture2D DiffuseMap;
			HydrogenTexture2D NormalMap;
			HydrogenTexture2D HeightMap;
			HydrogenTexture2D MetallicMap;
			HydrogenTexture2D RoughnessMap;
			HydrogenTexture2D AmbientOcclusionMap;
		} Material;
		HydrogenMesh Mesh;
		HydrogenBlendingInfo BlendingInfo;
		HydrogenTransform Transform;
		HydrogenModelRenderingComponents(void) = default;
	};

	class HYDROGEN_API HydrogenModel {
	private:
		//The main reason why I don't losely put the vars in the object is to make sure it is easy for the renderer class to access them without weird pointer offset and cast stuff.
		//My goal is to make the renderer do all the work, this is soley a wrapper for the model 
		HydrogenModelRenderingComponents Components;
	public:
		HydrogenModel(void);
		~HydrogenModel(void);
		void LoadModel(const std::string& path);
		HydrogenModelRenderingComponents* GetComponents(void);
		HydrogenTransform* GetTransform(void); //Short way of getting translation
		void ActivateTextureUnits(void);
	};

}

#endif
