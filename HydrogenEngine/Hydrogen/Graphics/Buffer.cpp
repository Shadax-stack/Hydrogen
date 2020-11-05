#include "Buffer.h"

#define Buffer Handle

namespace Hydrogen {

	HydrogenMemoryBuffer::HydrogenMemoryBuffer(void) {
		Size = 0;
		Data = nullptr;
	}
	HydrogenMemoryBuffer::~HydrogenMemoryBuffer(void) {
		if (Size != 0) {
			delete[] Data;
		}
	}

	HydrogenMemoryBuffer& HydrogenMemoryBuffer::operator<<(float val) {
		float data[4]{ 0.0f };
		data[0] = val;
		PushData(4, data, 0);
		return *this;
	}

	HydrogenMemoryBuffer& HydrogenMemoryBuffer::operator<<(int val) {
		int data[4]{ 0.0f };
		data[0] = val;
		PushData(4, data, 0);
		return *this;
	}

	HydrogenMemoryBuffer& HydrogenMemoryBuffer::operator<<(glm::vec3 val) {
		float data[4]{ 0.0f };
		data[0] = val.x;
		data[1] = val.y;
		data[2] = val.z;
		int remainder = Size % 16;
		PushData(16, data, remainder);
		return *this;
	}


	void HydrogenMemoryBuffer::PushData(size_t bytes, void* data, size_t resize) {
		char* copy = new char[Size + resize + bytes];
		if (Size != 0) {
			memcpy(copy, Data, Size);
		}
		Size += resize;
		memcpy(copy + Size, data, bytes);
		if (Size != 0) {
			delete[] Data;
		}
		Data = copy;
		Size += bytes;
	}

	HydrogenBuffer::HydrogenBuffer(void) : Size(0) {

	}

	void HydrogenBuffer::CreateIdentifier(void) {
		glCall(glGenBuffers(1, &Buffer));
	}

	void HydrogenBuffer::DeleteIdentifier(void) {
		glCall(glDeleteBuffers(1, &Buffer));
	}

	void HydrogenBuffer::ConfigureTarget(HydrogenBufferTarget target) {
		Target = target;
	}

	void HydrogenBuffer::Bind(void) {
		glCall(glBindBuffer(Target, Buffer));
	}

	void HydrogenBuffer::Unbind(void) {
		glCall(glBindBuffer(Target, 0));
	}

	void HydrogenBuffer::UploadData(size_t size, void* data) {
		Size = size;
		Bind();
		glCall(glBufferData(Target, Size, data, GL_STATIC_DRAW)); //Static draw for now until I find I need to pass that as an arg as well
		Unbind();
	}

	size_t HydrogenBuffer::GetSize(void) {
		return Size;
	}

	void HydrogenBuffer::UploadData(HydrogenMemoryBuffer* buf) {
		Size = buf->Size;
		Bind();
		glCall(glBufferData(Target, Size, buf->Data, GL_STATIC_DRAW)); //Static draw for now until I find I need to pass that as an arg as well
		Unbind();
	}

}