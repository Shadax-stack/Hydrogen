#ifndef HYDROGEN_BUFFER_H
#define HYDROGEN_BUFFER_H

#include "../Core/HydrogenEngine.h"
#include "Handle.h"

namespace Hydrogen {

	//Sort of like a vector but for rando types as well. IT's made for uploading and downloading to the buffer
	struct HYDROGEN_API HydrogenMemoryBuffer {
		char* Data; //Represent internal data as bytes
		size_t Size;
		HydrogenMemoryBuffer(void);
		~HydrogenMemoryBuffer(void);
		HydrogenMemoryBuffer& operator<<(float val);
		HydrogenMemoryBuffer& operator<<(int val);
		HydrogenMemoryBuffer& operator<<(glm::vec3 val);
	private:
		void PushData(size_t bytes, void* data, size_t resize);
	};

	enum HydrogenBufferTarget {
		HYDROGEN_BUFFER_TARGET_ARRAY = GL_ARRAY_BUFFER,
		HYDROGEN_BUFFER_TARGET_ELEMENT = GL_ELEMENT_ARRAY_BUFFER,
		HYDROGEN_BUFFER_TARGET_UNIFORM = GL_UNIFORM_BUFFER,
	};

	class HYDROGEN_API HydrogenBuffer : public HydrogenHandle {
	private:
		size_t Size; //Because getting buffer size via OpenGL calls is quite slow or something
		HydrogenBufferTarget Target; //I might just use something else instead of this
	public:
		HydrogenBuffer(void);
		void CreateIdentifier(void);
		void DeleteIdentifier(void);
		void SetIdentifier(unsigned int id) = delete;
		void ConfigureTarget(HydrogenBufferTarget target);
		void Bind(void);
		void Unbind(void); // nonstatic because target
		void UploadData(size_t size, void* data);
		void UploadData(HydrogenMemoryBuffer* buf);
		size_t GetSize(void);
	};

}

#endif
