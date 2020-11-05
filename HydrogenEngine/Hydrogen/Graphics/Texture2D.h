#ifndef HYDROGEN_TEXTURE_2D_H
#define HYDROGEN_TEXTURE_2D_H

#include "../Core/HydrogenEngine.h"
#include "Texture.h"

namespace Hydrogen {

	struct HydrogenTexture2DMemoryDataInfo : public HydrogenTextureMemoryDataInfo {
		int Width, Height;
	};


	struct HydrogenTexture2DInfo : public HydrogenTextureInfo {
		HydrogenTexture2DInfo(void) = default;
		int Width, Height;
	};

	class HydrogenTexture2D : public HydrogenTexture {
	private:
		HydrogenTexture2DInfo Texture2DInfo;
	public:
		HydrogenTexture2D(void) = default;
		void SetIdentifier(unsigned int id) = delete; //Bruh why would you want to modify the handle that's stupid
		void UploadData(HydrogenTextureInternalFormat texFormat, HydrogenTexturePixelFormat dataFormat, unsigned int width, unsigned int height, void* pixels = nullptr);
		void FilteringMode(HydrogenTextureFilteringMode min, HydrogenTextureFilteringMode mag);
		void SwizzleMasks(int* params);
		void CreateEmpty(HydrogenTextureInternalFormat texFormat, int width, int height);
		void Bind(void);
		static void Unbind(void);
		HydrogenTexture2DInfo* GetTexture2DInfo(void);
	};

}

#endif
