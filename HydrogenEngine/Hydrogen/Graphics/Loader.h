#ifndef HYDROGEN_LOADER_H
#define HYDROGEN_LOADER_H

#include "../Core/HydrogenEngine.h"
#include "Vertex.h"
#include <vector>

namespace Hydrogen {

	void LoadMesh(const char* path, std::vector<Hydrogen::HydrogenVertex>& verticies, std::vector<unsigned int>& indicies);
	unsigned char* LoadImage(const char* path, int* width, int* height, int* channels, int load = SOIL_LOAD_AUTO);
	void FreeImage(unsigned char* data);

	struct HYDROGEN_API HydrogenXMLElement {
		HydrogenXMLElement(void);
		std::string name;
		std::string content;
	};

	class HydrogenXMLReader {
	private:
		uint32_t Depth;
	public:
	};

}

#endif
