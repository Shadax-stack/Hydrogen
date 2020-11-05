#ifndef HYDROGEN_TEXTURE_CUBE_H
#define HYDROGEN_TEXTURE_CUBE_H

#include "../Core/HydrogenEngine.h"
#include "Texture.h"

namespace Hydrogen {

	enum HydrogenTextureCubemapTarget {
		HYDROGEN_TEXTURE_CUBE_MAP_POSITIVE_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		HYDROGEN_TEXTURE_CUBE_MAP_NEGATIVE_X,
		HYDROGEN_TEXTURE_CUBE_MAP_POSITIVE_Y,
		HYDROGEN_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		HYDROGEN_TEXTURE_CUBE_MAP_POSITIVE_Z,
		HYDROGEN_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	};

	struct HydrogenTextureCubeMemoryDataInfo : public HydrogenTextureMemoryDataInfo {
		HydrogenTextureCubemapTarget Target;
		int Width, Height;
	};

	class HYDROGEN_API HydrogenTextureCubemap : public HydrogenTexture {
	public:
		HydrogenTextureCubemap(void) = default;
		void UploadData(HydrogenTextureCubemapTarget target, HydrogenTextureInternalFormat texFormat, HydrogenTexturePixelFormat dataFormat, unsigned int width, unsigned int height, void* pixels);
		void FilteringMode(HydrogenTextureFilteringMode min, HydrogenTextureFilteringMode mag);
		void SwizzleMasks(int* params);
		void Bind(void);
		static void Unbind(void);
	};

}

#endif