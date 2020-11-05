#include "Texture2D.h"
#include <map>

#define Texture2D Handle

namespace Hydrogen {

	const int TextureQuality = 1; //Basically LOD
	constexpr const int LOD_2 = TextureQuality * TextureQuality;

	struct PixelInfo {
		int Width, Height;
		unsigned char* Pixels;
		~PixelInfo(void) {
			delete[] Pixels;
		}
	};

	void CreateLOD(int width, int height, unsigned char* pixels, int channels, PixelInfo* downsampled) {
		downsampled->Width = width / 2;
		downsampled->Height = height / 2;

		switch (channels) {
		case 1: {
			struct Color {
				unsigned char R;
				void operator+=(const Color& other) {
					R += other.R;
				}
				void operator/=(int lod) {
					R /= lod;
				}
			};
			Color* DownsampledPixels = new Color[downsampled->Width * downsampled->Height];
			downsampled->Pixels = (unsigned char*)DownsampledPixels;
			Color* Pixels = (Color*)pixels;
			for (int x = 0; x < downsampled->Width; x++) {
				for (int y = 0; y < downsampled->Height; y++) {
					int xStart = x * TextureQuality, yStart = y * TextureQuality;
					int xEnd = xStart + TextureQuality, yEnd = yStart + TextureQuality;
					Color* write = &DownsampledPixels[downsampled->Width * x + y];
					for (int x2 = xStart; x2 < xEnd; x2++) {
						for (int y2 = yStart; y2 < yEnd; y2++) {
							Color read = Pixels[downsampled->Width * x + y];
							*write += read;
						}
					}
					*write /= LOD_2;
				}
			}
		} break;
		case 2: {
			struct Color {
				unsigned char R, G;
				void operator+=(const Color& other) {
					R += other.R;
					G += other.G;
				}
				void operator/=(int lod) {
					R /= lod;
					G /= lod;
				}
			};
			Color* DownsampledPixels = new Color[downsampled->Width * downsampled->Height];
			downsampled->Pixels = (unsigned char*)DownsampledPixels;
			Color* Pixels = (Color*)pixels;
			for (int x = 0; x < downsampled->Width; x++) {
				for (int y = 0; y < downsampled->Height; y++) {
					int xStart = x * TextureQuality, yStart = y * TextureQuality;
					int xEnd = xStart + TextureQuality, yEnd = yStart + TextureQuality;
					Color* write = &DownsampledPixels[downsampled->Width * x + y];
					for (int x2 = xStart; x2 < xEnd; x2++) {
						for (int y2 = yStart; y2 < yEnd; y2++) {
							Color read = Pixels[downsampled->Width * x + y];
							*write += read;
						}
					}
					*write /= LOD_2;
				}
			}
		} break;
		case 3: {
			struct Color {
				unsigned char R, G, B;
				void operator+=(const Color& other) {
					R += other.R;
					G += other.G;
					B += other.B;
				}
				void operator/=(int lod) {
					R /= lod;
					G /= lod;
					B /= lod;
				}
			};
			Color* DownsampledPixels = new Color[downsampled->Width * downsampled->Height];
			downsampled->Pixels = (unsigned char*)DownsampledPixels;
			Color* Pixels = (Color*)pixels;
			for (int x = 0; x < downsampled->Width; x++) {
				for (int y = 0; y < downsampled->Height; y++) {
					int xStart = x * TextureQuality, yStart = y * TextureQuality;
					int xEnd = xStart + TextureQuality, yEnd = yStart + TextureQuality;
					Color* write = &DownsampledPixels[downsampled->Width * x + y];
					for (int x2 = xStart; x2 < xEnd; x2++) {
						for (int y2 = yStart; y2 < yEnd; y2++) {
							Color read = Pixels[downsampled->Width * x + y];
							*write += read;
						}
					}
					*write /= LOD_2;
				}
			}
		} break;
		case 4: {
			struct Color {
				unsigned char R, G, B, A;
				void operator+=(const Color& other) {
					R += other.R;
					G += other.G;
					B += other.B;
					A += other.A;
				}
				void operator/=(int lod) {
					R /= lod;
					G /= lod;
					B /= lod;
					A /= lod;
				}
			};
			Color* DownsampledPixels = new Color[downsampled->Width * downsampled->Height];
			downsampled->Pixels = (unsigned char*)DownsampledPixels;
			Color* Pixels = (Color*)pixels;
			for (int x = 0; x < downsampled->Width; x++) {
				for (int y = 0; y < downsampled->Height; y++) {
					int xStart = x * TextureQuality, yStart = y * TextureQuality;
					int xEnd = xStart + TextureQuality, yEnd = yStart + TextureQuality;
					Color* write = &DownsampledPixels[downsampled->Width * x + y];
					for (int x2 = xStart; x2 < xEnd; x2++) {
						for (int y2 = yStart; y2 < yEnd; y2++) {
							Color read = Pixels[downsampled->Width * x + y];
							*write += read;
						}
					}
					*write /= LOD_2;
				}
			}
		} break;
		default:
			break;
		}
	}

	void HydrogenTexture2D::UploadData(HydrogenTextureInternalFormat texFormat, HydrogenTexturePixelFormat dataFormat, unsigned int width, unsigned int height, void* pixels) {
		Texture2DInfo.InternalFormat = texFormat;
		Texture2DInfo.Width = width;
		Texture2DInfo.Height = height;
		unsigned int format, type, channels;
		PixelFormat(dataFormat, &format, &type, &channels);
		PixelInfo info;
		CreateLOD(width, height, (unsigned char*)pixels, channels, &info); //TODO: Fix this
		Bind();
		glCall(glTexImage2D(HYDROGEN_TEXTURE_TARGET_TEXTURE_2D, 0, Texture2DInfo.InternalFormat, Texture2DInfo.Width, Texture2DInfo.Height, 0, format, type, pixels));
		glCall(glTexParameteri(HYDROGEN_TEXTURE_TARGET_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		glCall(glTexParameteri(HYDROGEN_TEXTURE_TARGET_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		Unbind();
	}

	void HydrogenTexture2D::FilteringMode(HydrogenTextureFilteringMode min, HydrogenTextureFilteringMode mag) {
		Bind();
		if (min == HYDROGEN_TEXTURE_FILTERING_MODE_NEAREST_MIPMAP_NEAREST
			|| min == HYDROGEN_TEXTURE_FILTERING_MODE_NEAREST_MIPMAP_LINEAR
			|| min == HYDROGEN_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_NEAREST
			|| min == HYDROGEN_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_LINEAR) {
			glCall(glGenerateMipmap(HYDROGEN_TEXTURE_TARGET_TEXTURE_2D));
		}
		glCall(glTexParameteri(HYDROGEN_TEXTURE_TARGET_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min));
		glCall(glTexParameteri(HYDROGEN_TEXTURE_TARGET_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag));
		Unbind();
	}

	void HydrogenTexture2D::SwizzleMasks(int* params) {
		Bind();
		glCall(glTexParameteriv(HYDROGEN_TEXTURE_TARGET_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, params));
		Unbind();
	}

	void HydrogenTexture2D::Bind(void) {
		glCall(glBindTexture(HYDROGEN_TEXTURE_TARGET_TEXTURE_2D, Texture2D));
	}

	void HydrogenTexture2D::Unbind(void) {
		glCall(glBindTexture(HYDROGEN_TEXTURE_TARGET_TEXTURE_2D, 0));
	}

	void HydrogenTexture2D::CreateEmpty(HydrogenTextureInternalFormat texFormat, int width, int height) {
		Texture2DInfo.InternalFormat = texFormat;
		Texture2DInfo.Width = width;
		Texture2DInfo.Height = height;
		unsigned int format, type;
		CreateEmptyFormat(texFormat, &format, &type);
		Bind();
		glCall(glTexImage2D(HYDROGEN_TEXTURE_TARGET_TEXTURE_2D, 0, Texture2DInfo.InternalFormat, Texture2DInfo.Width, Texture2DInfo.Height, 0, format, type, nullptr));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		Unbind();
	}

	HydrogenTexture2DInfo* HydrogenTexture2D::GetTexture2DInfo(void) {
		return &Texture2DInfo;
	}

}