#include "Mesh.h"

#define VertexArray Handle

namespace Hydrogen {

	extern bool LoadedOpenGL;

	HydrogenMesh::HydrogenMesh(void) {
		Buffers.VertexBuffer.ConfigureTarget(HYDROGEN_BUFFER_TARGET_ARRAY);
		Buffers.ElementBuffer.ConfigureTarget(HYDROGEN_BUFFER_TARGET_ELEMENT);
		if (LoadedOpenGL) {
			CreateIdentifier();
		}
	}

	HydrogenMesh::~HydrogenMesh(void) {
		DeleteIdentifier();
	}

	//This also creates the VBOs, since their existnace has no point with the VAO. Same for delete func
	void HydrogenMesh::CreateIdentifier(void) {
		glCall(glGenVertexArrays(1, &VertexArray));
		Buffers.VertexBuffer.CreateIdentifier();
		Buffers.ElementBuffer.CreateIdentifier();
		Bind();
		Buffers.VertexBuffer.Bind();
		Buffers.ElementBuffer.Bind();
		glCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 56, (const void*)0));  //Position
		glCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 56, (const void*)12)); //Normal
		glCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 56, (const void*)24)); //TexCoord
		glCall(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 56, (const void*)32)); //Tangent
		glCall(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 56, (const void*)44)); //BiTangent
		glCall(glEnableVertexAttribArray(0));
		glCall(glEnableVertexAttribArray(1));
		glCall(glEnableVertexAttribArray(2));
		glCall(glEnableVertexAttribArray(3));
		glCall(glEnableVertexAttribArray(4));
		Unbind();
		Buffers.VertexBuffer.Unbind();
		Buffers.ElementBuffer.Unbind();
	}

	//YOU are responsible for deleting the object on your own
	void HydrogenMesh::DeleteIdentifier(void) {
		glCall(glDeleteVertexArrays(1, &VertexArray));
		Buffers.VertexBuffer.DeleteIdentifier();
		Buffers.ElementBuffer.DeleteIdentifier();
	}

	void HydrogenMesh::UploadData(const std::vector<HydrogenVertex>& vertices, const std::vector<unsigned int>& indicies) {
		Buffers.VertexBuffer.UploadData(vertices.size() * sizeof(HydrogenVertex), (void*)&vertices[0]);
		Buffers.ElementBuffer.UploadData(indicies.size() * sizeof(unsigned int), (void*)&indicies[0]);
	}

	unsigned int HydrogenMesh::GetElementCount(void) {
		return Buffers.ElementBuffer.GetSize() / sizeof(unsigned int);
	}

	unsigned int HydrogenMesh::GetVertexCount(void) {
		return Buffers.VertexBuffer.GetSize() / sizeof(HydrogenVertex);
	}

	void HydrogenMesh::Bind(void) {
		glBindVertexArray(VertexArray);
	}

	void HydrogenMesh::Unbind(void) {
		glBindVertexArray(0);
	}

}