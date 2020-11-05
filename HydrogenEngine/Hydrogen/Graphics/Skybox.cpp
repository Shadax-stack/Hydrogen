#include "Skybox.h"
#include <iostream>
#include <fstream>

namespace Hydrogen {

	HydrogenSkybox::HydrogenSkybox(void) {
		Components.Skybox.CreateIdentifier();
	}

	HydrogenSkybox::~HydrogenSkybox(void) {
		Components.Skybox.DeleteIdentifier();
	}

	void HydrogenSkybox::LoadSkybox(const std::string& path) {
		std::ifstream SkyboxFile(path);
		//Right, so the file contains paths to each part of the skybox seperated by new lines
		for (int face = 0; face < 6; face++) {
			std::string image;
			std::getline(SkyboxFile, image);
			int Width, Height, Channels;
			unsigned char* Pixels = LoadImage(image.c_str(), &Width, &Height, &Channels, SOIL_LOAD_RGB); //I doubt anyone will really use anything besides RGB
			Components.Skybox.UploadData((HydrogenTextureCubemapTarget)((int)HYDROGEN_TEXTURE_CUBE_MAP_POSITIVE_X + face), HYDROGEN_TEXTURE_INTERNAL_FORMAT_RGB_8, HYDROGEN_TEXTURE_PIXEL_FORMAT_RGB_UNSIGNED_BYTE, Width, Height, Pixels);
			FreeImage(Pixels);
		}
		Components.Skybox.FilteringMode(HYDROGEN_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_LINEAR, HYDROGEN_TEXTURE_FILTERING_MODE_LINEAR);
	}

	void HydrogenSkybox::ActivateTextureUnits(void) {
		glCall(glActiveTexture(GL_TEXTURE0));
		Components.Skybox.Bind();
	}

}