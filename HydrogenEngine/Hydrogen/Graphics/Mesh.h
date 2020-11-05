#ifndef HYDROGEN_MESH_H
#define HYDROGEN_MESH_H

#include "../Core/HydrogenEngine.h"
#include "Handle.h"
#include "Vertex.h"
#include "Buffer.h"
#include <vector>

namespace Hydrogen {

	struct HydrogenMeshBuffers {
		HydrogenBuffer VertexBuffer;
		HydrogenBuffer ElementBuffer;
	};

	class HYDROGEN_API HydrogenMesh : public HydrogenHandle {
	private:
		HydrogenMeshBuffers Buffers;
	public:
		HydrogenMesh(void);
		~HydrogenMesh(void);
		void CreateIdentifier(void);
		void DeleteIdentifier(void);
		void SetIdentifier(unsigned int id) = delete; 
		void UploadData(const std::vector<HydrogenVertex>& vertices, const std::vector<unsigned int>& indicies);
		unsigned int GetElementCount(void);
		unsigned int GetVertexCount(void);
		void Bind(void);
		static void Unbind(void);
	};

}

#endif
