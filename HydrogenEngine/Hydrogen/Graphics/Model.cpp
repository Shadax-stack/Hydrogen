#include "Model.h"
#include "Loader.h"
#include <fstream>

//pixels is raw data, num is number of pixels, found from width * height
Hydrogen::HydrogenBlendingInfo CheckBlending(unsigned char* pixels, int num) {
	for (int index = 0; index < num; index++) {
		unsigned char alpha = pixels[(index) * 4 + 4]; //We add 4 so it reads alpha only. A better solution would be to use a struct for this. 
		if (alpha != 255 || alpha != 0) {
			return Hydrogen::HYDROGEN_BLENDING_INFO_TRANSLUCENT;
		}
	}
	return Hydrogen::HYDROGEN_BLENDING_INFO_TRANSPARENT;
}

namespace Hydrogen {

	HydrogenModel::HydrogenModel(void) {
		Components.Material.DiffuseMap.CreateIdentifier();
		Components.Material.NormalMap.CreateIdentifier();
		Components.Material.HeightMap.CreateIdentifier();
		Components.Material.MetallicMap.CreateIdentifier();
		Components.Material.RoughnessMap.CreateIdentifier();
		Components.Material.AmbientOcclusionMap.CreateIdentifier();
	}

	HydrogenModel::~HydrogenModel(void) {
		Components.Material.DiffuseMap.DeleteIdentifier();
		Components.Material.NormalMap.DeleteIdentifier();
		Components.Material.HeightMap.DeleteIdentifier();
		Components.Material.MetallicMap.DeleteIdentifier();
		Components.Material.RoughnessMap.DeleteIdentifier();
		Components.Material.AmbientOcclusionMap.DeleteIdentifier();
	}

	void HydrogenModel::LoadModel(const std::string& path) {
		std::ifstream file(path);
		std::string paths[7];
		int index = 0;
		for (std::string line; std::getline(file, line); index++) {
			paths[index] = line;
			/*
			Basically the file will consist of paths to certain parts of the model
			I will replace this with Json or XML but here is it's current format:
			Mesh
			Diffuse
			Normal
			Metalness
			Roughness
			Ambient Occlusion
			*/
			//std::cout << line << '\n';
		}
		file.close();
		std::vector<HydrogenVertex> verticies;
		std::vector<unsigned int> indicies;
		LoadMesh(paths[0].c_str(), verticies, indicies);
		Components.Mesh.UploadData(verticies, indicies);
		int width, height, channels;
		unsigned char* texture;
		texture = LoadImage(paths[1].c_str(), &width, &height, &channels);
		if (channels < 4) { //TODO: change this to support 2D texture, but really, it would probally not exist in normal cases. I may just "upscale" the channels to RGB
			Components.Material.DiffuseMap.UploadData(HYDROGEN_TEXTURE_INTERNAL_FORMAT_RGB_8, HYDROGEN_TEXTURE_PIXEL_FORMAT_RGB_UNSIGNED_BYTE, width, height, texture);
			Components.BlendingInfo = HYDROGEN_BLENDING_INFO_OPAQUE;
		}
		else {
			Components.Material.DiffuseMap.UploadData(HYDROGEN_TEXTURE_INTERNAL_FORMAT_RGBA_8, HYDROGEN_TEXTURE_PIXEL_FORMAT_RGBA_UNSIGNED_BYTE, width, height, texture);
			Components.BlendingInfo = CheckBlending(texture, width * height);
		}
		FreeImage(texture);
		texture = LoadImage(paths[2].c_str(), &width, &height, &channels, SOIL_LOAD_RGB);
		Components.Material.NormalMap.UploadData(HYDROGEN_TEXTURE_INTERNAL_FORMAT_RGB_8, HYDROGEN_TEXTURE_PIXEL_FORMAT_RGB_UNSIGNED_BYTE, width, height, texture);
		FreeImage(texture);
		texture = LoadImage(paths[3].c_str(), &width, &height, &channels, SOIL_LOAD_L);
		Components.Material.HeightMap.UploadData(HYDROGEN_TEXTURE_INTERNAL_FORMAT_R_8, HYDROGEN_TEXTURE_PIXEL_FORMAT_R_UNSIGNED_BYTE, width, height, texture);
		FreeImage(texture);
		texture = LoadImage(paths[4].c_str(), &width, &height, &channels, SOIL_LOAD_L);
		Components.Material.MetallicMap.UploadData(HYDROGEN_TEXTURE_INTERNAL_FORMAT_R_8, HYDROGEN_TEXTURE_PIXEL_FORMAT_R_UNSIGNED_BYTE, width, height, texture);
		FreeImage(texture);
		texture = LoadImage(paths[5].c_str(), &width, &height, &channels, SOIL_LOAD_L);
		Components.Material.RoughnessMap.UploadData(HYDROGEN_TEXTURE_INTERNAL_FORMAT_R_8, HYDROGEN_TEXTURE_PIXEL_FORMAT_R_UNSIGNED_BYTE, width, height, texture);
		FreeImage(texture);
		texture = LoadImage(paths[6].c_str(), &width, &height, &channels, SOIL_LOAD_L);
		Components.Material.AmbientOcclusionMap.UploadData(HYDROGEN_TEXTURE_INTERNAL_FORMAT_R_8, HYDROGEN_TEXTURE_PIXEL_FORMAT_R_UNSIGNED_BYTE, width, height, texture);
		FreeImage(texture);
		Components.Material.DiffuseMap.FilteringMode(HYDROGEN_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_LINEAR, HYDROGEN_TEXTURE_FILTERING_MODE_LINEAR);
		Components.Material.NormalMap.FilteringMode(HYDROGEN_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_LINEAR, HYDROGEN_TEXTURE_FILTERING_MODE_LINEAR);
		Components.Material.HeightMap.FilteringMode(HYDROGEN_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_LINEAR, HYDROGEN_TEXTURE_FILTERING_MODE_LINEAR);
		Components.Material.MetallicMap.FilteringMode(HYDROGEN_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_LINEAR, HYDROGEN_TEXTURE_FILTERING_MODE_LINEAR);
		Components.Material.RoughnessMap.FilteringMode(HYDROGEN_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_LINEAR, HYDROGEN_TEXTURE_FILTERING_MODE_LINEAR);
		Components.Material.AmbientOcclusionMap.FilteringMode(HYDROGEN_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_LINEAR, HYDROGEN_TEXTURE_FILTERING_MODE_LINEAR);
	}

	HydrogenModelRenderingComponents* HydrogenModel::GetComponents(void) {
		return &Components;
	}

	HydrogenTransform* HydrogenModel::GetTransform(void) {
		return &GetComponents()->Transform; 
	}

	void HydrogenModel::ActivateTextureUnits(void) {
		glCall(glActiveTexture(GL_TEXTURE0));
		Components.Material.DiffuseMap.Bind();
		glCall(glActiveTexture(GL_TEXTURE1));
		Components.Material.NormalMap.Bind();
		glCall(glActiveTexture(GL_TEXTURE2));
		Components.Material.HeightMap.Bind();
		glCall(glActiveTexture(GL_TEXTURE3));
		Components.Material.MetallicMap.Bind();
		glCall(glActiveTexture(GL_TEXTURE4));
		Components.Material.RoughnessMap.Bind();
		glCall(glActiveTexture(GL_TEXTURE5));
		Components.Material.AmbientOcclusionMap.Bind();
	}

}