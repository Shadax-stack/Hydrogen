#include "LinearAlgebra.h"

namespace Hydrogen {

	HydrogenVector3f::HydrogenVector3f(float xval, float yval, float zval) : x(xval), y(yval), z(zval) {}
	HydrogenVector3f::HydrogenVector3f(float val) : HydrogenVector3f(0.0f, 0.0f, 0.0f) {}
	HydrogenVector3f::HydrogenVector3f(void) : HydrogenVector3f(0.0f) {}


}

std::ostream& operator<<(std::ostream& os, glm::vec3 val) {
	return os << '(' << val.x << ", " << val.y << ", " << val.z << ')';
}
