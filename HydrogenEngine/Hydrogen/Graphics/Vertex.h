#ifndef HYDROGEN_VERTEX_H
#define HYDROGEN_VERTEX_H

#include "../Core/HydrogenEngine.h"

namespace Hydrogen {

	struct HYDROGEN_API HydrogenVertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
		HydrogenVertex(void);
		//TODO: Make pass by ref
		HydrogenVertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 uv, glm::vec3 tan = glm::vec3(0.0f), glm::vec3 bitan = glm::vec3(0.0f));
		~HydrogenVertex(void) = default;
		friend std::ostream& operator<<(std::ostream& os, const HydrogenVertex& vertex);
	};

	std::ostream& operator<<(std::ostream& os, const HydrogenVertex& vertex);

}

#endif
