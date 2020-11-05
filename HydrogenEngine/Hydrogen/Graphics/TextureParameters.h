#ifndef HYDROGEN_TEXTURE_PARAMETERS_H
#define HYDROGEN_TEXTURE_PARAMETERS_H

#include "../Core/HydrogenEngine.h"

namespace Hydrogen {

	#define HYDROGEN_TEXTURE_FILTERING_LINEAR
	#define HYDROGEN_TEXTURE_FILTERING_NEAREST

	struct HYDROGEN_API HydrogenTextureParameters {
		bool Mipmap;
		int Filtering;
		HydrogenTextureParameters(void);
		~HydrogenTextureParameters(void) = default;
	};

	enum HydrogenTextureFiltering {
		HYDROGEN_TEXTURE_FILITERING_NEAREST, //Nearest 
		HYDROGEN_TEXTURE_FILITERING_LINEAR,  //Linear interpolation
		HYDROGEN_TEXTURE_FILITERING_BILINEAR,//+ mipmaps (+ anistorpic filterting if supported)
		HYDROGEN_TEXTURE_FILITERING_TRILINEAR,//interpolate betweeen mipmaps
	};

	//We store these paramters to help resize a texture 
	struct HydrogenTextureFormat {
		unsigned int InternalFormat;
		unsigned int Format;
		unsigned int Type;
	};

}

#endif