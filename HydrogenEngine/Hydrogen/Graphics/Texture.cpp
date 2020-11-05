#include "Texture.h"

#define Texture Handle

namespace Hydrogen {

	//I could use an array but I wont for now just in case an array might not work in the future
	void HydrogenTexture::PixelFormat(HydrogenTexturePixelFormat format, unsigned int* texFormat, unsigned int* type, unsigned int* channels) {
		switch (format) {
		case HYDROGEN_TEXTURE_PIXEL_FORMAT_R_UNSIGNED_BYTE:
			*texFormat = GL_RED;
			*type = GL_UNSIGNED_BYTE;
			if (channels) *channels = 1;
			return;
		case HYDROGEN_TEXTURE_PIXEL_FORMAT_RG_UNSIGNED_BYTE:
			*texFormat = GL_RG;
			*type = GL_UNSIGNED_BYTE;
			if (channels) *channels = 2;
			return;
		case HYDROGEN_TEXTURE_PIXEL_FORMAT_RGB_UNSIGNED_BYTE:
			*texFormat = GL_RGB;
			*type = GL_UNSIGNED_BYTE;
			if (channels) *channels = 3;
			return;
		case HYDROGEN_TEXTURE_PIXEL_FORMAT_RGBA_UNSIGNED_BYTE:
			*texFormat = GL_RGBA;
			*type = GL_UNSIGNED_BYTE;
			if (channels) *channels = 4;
			return;
		case HYDROGEN_TEXTURE_PIXEL_FORMAT_R_FLOAT:
			*texFormat = GL_RED;
			*type = GL_FLOAT;
			if (channels) *channels = 1;
			return;
		case HYDROGEN_TEXTURE_PIXEL_FORMAT_RG_FLOAT:
			*texFormat = GL_RG;
			*type = GL_FLOAT;
			if (channels) *channels = 2;
			return;
		case HYDROGEN_TEXTURE_PIXEL_FORMAT_RGB_FLOAT:
			*texFormat = GL_RGB;
			*type = GL_FLOAT;
			if (channels) *channels = 3;
			return;
		case HYDROGEN_TEXTURE_PIXEL_FORMAT_RGBA_FLOAT:
			*texFormat = GL_RGBA;
			*type = GL_FLOAT;
			if (channels) *channels = 4;
			return;
		default:
			return;
		}
	}

	void HydrogenTexture::CreateIdentifier(void) {
		glCall(glGenTextures(1, &Texture));
	}

	void HydrogenTexture::DeleteIdentifier(void) {
		glCall(glDeleteTextures(1, &Texture));

	}

	/*
			HYDROGEN_TEXTURE_INTERNAL_FORMAT_R_8 = GL_R8,
		HYDROGEN_TEXTURE_INTERNAL_FORMAT_RG_8 = GL_RG8,
		HYDROGEN_TEXTURE_INTERNAL_FORMAT_RGB_8 = GL_RGB8,
		HYDROGEN_TEXTURE_INTERNAL_FORMAT_RGBA_8 = GL_RGBA8,
		HYDROGEN_TEXTURE_INTERNAL_FORMAT_R_16F = GL_R16F,
		HYDROGEN_TEXTURE_INTERNAL_FORMAT_RG_16F = GL_RG16F,
		HYDROGEN_TEXTURE_INTERNAL_FORMAT_RGB_16F = GL_RGB16F,
		HYDROGEN_TEXTURE_INTERNAL_FORMAT_RGBA_16F = GL_RGBA16F,
		HYDROGEN_TEXTURE_INTERNAL_FORMAT_R_32F = GL_R32F,
		HYDROGEN_TEXTURE_INTERNAL_FORMAT_RG_32F = GL_RG32F,
		HYDROGEN_TEXTURE_INTERNAL_FORMAT_RGB_32F = GL_RGB32F,
		HYDROGEN_TEXTURE_INTERNAL_FORMAT_RGBA_32F = GL_RGBA32F,
		HYDROGEN_TEXTURE_INTERNAL_FORMAT_DEPTH_16F = 0,
		HYDROGEN_TEXTURE_INTERNAL_FORMAT_DEPTH_32F = 0,
		
		
		HYDROGEN_TEXTURE_INTERNAL_FORMAT_DEPTH_16 = GL_DEPTH_COMPONENT16,
		HYDROGEN_TEXTURE_INTERNAL_FORMAT_DEPTH_32 = GL_DEPTH_COMPONENT32,
		HYDROGEN_TEXTURE_INTERNAL_FORMAT_DEPTH_16F = GL_DEPTH_COMPONENT,
		HYDROGEN_TEXTURE_INTERNAL_FORMAT_DEPTH_32F = GL_DEPTH_COMPONENT32F,

		*/

	void HydrogenTexture::CreateEmptyFormat(HydrogenTextureInternalFormat internalFormat, unsigned int* texFormat, unsigned int* type) {
		switch (internalFormat)	{
		case HYDROGEN_TEXTURE_INTERNAL_FORMAT_R_8:
			*texFormat = GL_RED;
			*type = GL_UNSIGNED_BYTE;
			return;
		case HYDROGEN_TEXTURE_INTERNAL_FORMAT_RG_8:
			*texFormat = GL_RG;
			*type = GL_UNSIGNED_BYTE;
			return;
		case HYDROGEN_TEXTURE_INTERNAL_FORMAT_RGB_8:
			*texFormat = GL_RGB;
			*type = GL_UNSIGNED_BYTE;
			return;
		case HYDROGEN_TEXTURE_INTERNAL_FORMAT_RGBA_8:
			*texFormat = GL_RGBA;
			*type = GL_UNSIGNED_BYTE;
			return;
		case HYDROGEN_TEXTURE_INTERNAL_FORMAT_R_16F:
			*texFormat = GL_RED;
			*type = GL_HALF_FLOAT;
			return;
		case HYDROGEN_TEXTURE_INTERNAL_FORMAT_RG_16F:
			*texFormat = GL_RG;
			*type = GL_HALF_FLOAT;
			return;
		case HYDROGEN_TEXTURE_INTERNAL_FORMAT_RGB_16F:
			*texFormat = GL_RGB;
			*type = GL_HALF_FLOAT;
			return;
		case HYDROGEN_TEXTURE_INTERNAL_FORMAT_RGBA_16F:
			*texFormat = GL_RGBA;
			*type = GL_HALF_FLOAT;
			return;
		case HYDROGEN_TEXTURE_INTERNAL_FORMAT_R_32F:
			*texFormat = GL_RED;
			*type = GL_FLOAT;
			return;
		case HYDROGEN_TEXTURE_INTERNAL_FORMAT_RG_32F:
			*texFormat = GL_RG;
			*type = GL_FLOAT;
			return;
		case HYDROGEN_TEXTURE_INTERNAL_FORMAT_RGB_32F:
			*texFormat = GL_RGB;
			*type = GL_FLOAT;
			return;
		case HYDROGEN_TEXTURE_INTERNAL_FORMAT_RGBA_32F:
			*texFormat = GL_RGBA;
			*type = GL_FLOAT;
			return;
		case HYDROGEN_TEXTURE_INTERNAL_FORMAT_DEPTH_16:
			*texFormat = GL_DEPTH_COMPONENT;
			*type = GL_UNSIGNED_SHORT;
			return;
		case HYDROGEN_TEXTURE_INTERNAL_FORMAT_DEPTH_16F:
			*texFormat = GL_DEPTH_COMPONENT;
			*type = GL_HALF_FLOAT;
			return;
		case HYDROGEN_TEXTURE_INTERNAL_FORMAT_DEPTH_32:
			*texFormat = GL_DEPTH_COMPONENT;
			*type = GL_UNSIGNED_INT;
			return;
		case HYDROGEN_TEXTURE_INTERNAL_FORMAT_DEPTH_32F:
			*texFormat = GL_DEPTH_COMPONENT;
			*type = GL_FLOAT;
			return;
		default:
			return;
		}
	}

}