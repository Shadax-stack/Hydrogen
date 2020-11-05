#include "TextureCubemap.h"

#define TextureCubemap Handle

namespace Hydrogen {

	void HydrogenTextureCubemap::Bind(void) {
		glCall(glBindTexture(HYDROGEN_TEXTURE_TARGET_TEXTURE_CUBEMAP, TextureCubemap));
	}

	void HydrogenTextureCubemap::Unbind(void) {
		glCall(glBindTexture(HYDROGEN_TEXTURE_TARGET_TEXTURE_CUBEMAP, 0));
	}

	void HydrogenTextureCubemap::UploadData(HydrogenTextureCubemapTarget target, HydrogenTextureInternalFormat texFormat, HydrogenTexturePixelFormat dataFormat, unsigned int width, unsigned int height, void* pixels) {
		Bind();
		unsigned int format, type;
		PixelFormat(dataFormat, &format, &type);
		glCall(glTexImage2D(target, 0, texFormat, width, height, 0, format, type, pixels));
		Unbind();
	}

	void HydrogenTextureCubemap::FilteringMode(HydrogenTextureFilteringMode min, HydrogenTextureFilteringMode mag) {
		Bind();
		//May as well set wrapping params here, many people will really not care about this anyway
		glCall(glTexParameteri(HYDROGEN_TEXTURE_TARGET_TEXTURE_CUBEMAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		glCall(glTexParameteri(HYDROGEN_TEXTURE_TARGET_TEXTURE_CUBEMAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		glCall(glTexParameteri(HYDROGEN_TEXTURE_TARGET_TEXTURE_CUBEMAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		if (   min == HYDROGEN_TEXTURE_FILTERING_MODE_NEAREST_MIPMAP_NEAREST
			|| min == HYDROGEN_TEXTURE_FILTERING_MODE_NEAREST_MIPMAP_LINEAR
			|| min == HYDROGEN_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_NEAREST
			|| min == HYDROGEN_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_LINEAR) {
			glCall(glGenerateMipmap(HYDROGEN_TEXTURE_TARGET_TEXTURE_CUBEMAP));
		}
		glCall(glTexParameteri(HYDROGEN_TEXTURE_TARGET_TEXTURE_CUBEMAP, GL_TEXTURE_MIN_FILTER, min));
		glCall(glTexParameteri(HYDROGEN_TEXTURE_TARGET_TEXTURE_CUBEMAP, GL_TEXTURE_MAG_FILTER, mag));
		Unbind();
	}
	void HydrogenTextureCubemap::SwizzleMasks(int* params) {
		Bind();
		glCall(glTexParameteriv(HYDROGEN_TEXTURE_TARGET_TEXTURE_CUBEMAP, GL_TEXTURE_SWIZZLE_RGBA, params));
		Unbind();
	}

}