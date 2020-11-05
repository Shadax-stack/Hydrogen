#include "Loader.h"

namespace Hydrogen {

	glm::vec3 Convert(aiVector3D vec) {
		return glm::vec3(vec.x, vec.y, vec.z);
	}

	glm::vec2 Convert(aiVector2D vec) {
		return glm::vec2(vec.x, vec.y);
	}

	void LoadMesh(const char* path, std::vector<Hydrogen::HydrogenVertex>& verticies, std::vector<unsigned int>& indicies) {
		const aiScene* model = aiImportFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		for (int meshIndex = 0; meshIndex < model->mNumMeshes; meshIndex++) {
			aiMesh* mesh = model->mMeshes[meshIndex];
			verticies.reserve(mesh->mNumVertices);
			for (int vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++) {
				Hydrogen::HydrogenVertex vertex;
				vertex.Position = Convert(mesh->mVertices[vertexIndex]);
				vertex.TexCoords = Convert(mesh->mTextureCoords[0][vertexIndex]);
				vertex.Normal = Convert(mesh->mNormals[vertexIndex]);
				vertex.Tangent = Convert(mesh->mTangents[vertexIndex]);
				vertex.Bitangent = Convert(mesh->mBitangents[vertexIndex]);
				verticies.push_back(vertex);
			}
			for (int faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++) {
				aiFace face = mesh->mFaces[faceIndex];
				for (int indexIndex = 0; indexIndex < face.mNumIndices; indexIndex++) { //Bruh this counter name
					indicies.push_back(face.mIndices[indexIndex]);
				}
			}
		}
		aiReleaseImport(model);
	}

	//Image formats usually store data in terms of unsigned bytes. 
//However if I needed to load something like a LUT or prerendered shadow map, I would use my custom file format that stores the info
	unsigned char* LoadImage(const char* path, int* width, int* height, int* channels, int load) {
		return SOIL_load_image(path, width, height, channels, load);
	}

	void FreeImage(unsigned char* data) {
		SOIL_free_image_data(data);
	}


	HydrogenXMLElement::HydrogenXMLElement(void) {

	}
}