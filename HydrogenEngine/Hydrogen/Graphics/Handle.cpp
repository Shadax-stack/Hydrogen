#include "Handle.h"

namespace Hydrogen {

	HydrogenHandle::HydrogenHandle(void) : Handle(0) {}

	void HydrogenHandle::CreateIdentifier(void) {
		Hydrogen::ThrowException("The default handle class doesn't implement identifiers");
	}

	void HydrogenHandle::DeleteIdentifier(void) {
		Hydrogen::ThrowException("The default handle class doesn't implement identifiers");
	}

	unsigned int HydrogenHandle::RetrieveIdentifier(void) {
		return Handle;
	}

	unsigned int HydrogenHandle::GetIdentifier(void) {
		return Handle;
	}

	void HydrogenHandle::SetIdentifier(unsigned int id) {
		Handle = id;
	}

}