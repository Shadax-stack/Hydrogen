#include "Vertex.h"

namespace Hydrogen {

	HydrogenVertex::HydrogenVertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 uv, glm::vec3 tan, glm::vec3 bitan) : Position(pos), Normal(norm), TexCoords(uv), Tangent(tan), Bitangent(bitan) {}

	HydrogenVertex::HydrogenVertex(void) : Position(0.0f), Normal(0.0f, 1.0f, 0.0f), TexCoords(0.0f), Tangent(1.0f, 0.0f, 0.0f), Bitangent(0.0f, 0.0f, 1.0f) {}

	std::ostream& operator<<(std::ostream& os, const HydrogenVertex& vertex) {
		return os << "HydrogenVertex("
			<< "glm::vec3" << '(' << vertex.Position.x << ", " << vertex.Position.y << ", " << vertex.Position.z << "), "
			<< "glm::vec3" << '(' << vertex.Normal.x << ", " << vertex.Normal.y << ", " << vertex.Normal.z << "), "
			<< "glm::vec2" << '(' << vertex.TexCoords.x * 4.0f << ", " << vertex.TexCoords.y * 4.0f << "), "
			<< "glm::vec3" << '(' << vertex.Tangent.x << ", " << vertex.Tangent.y << ", " << vertex.Tangent.z << "), "
			<< "glm::vec3" << '(' << vertex.Bitangent.x << ", " << vertex.Bitangent.y << ", " << vertex.Bitangent.z << ")),";
		
	}
}