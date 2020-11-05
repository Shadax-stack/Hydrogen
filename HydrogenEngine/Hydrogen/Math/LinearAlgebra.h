#ifndef HYDROGEN_LINEAR_ALGEBRA_H
#define HYDROGEN_LINEAR_ALGEBRA_H

#include "../Core/HydrogenEngine.h"

namespace Hydrogen {

	struct HYDROGEN_API HydrogenVector3f {
		float x, y, z;
		HydrogenVector3f(float xval, float yval, float zval);
		HydrogenVector3f(float val);
		HydrogenVector3f(void);
	};

	struct HYDROGEN_API HydrogenMatrix4x4f {
		float Matrix[4][4];
	};

	template<typename InputType, typename OutputType>
	OutputType ConvertMathType(const InputType & input) {
		return *(OutputType*)&input;
	}


}

HYDROGEN_API std::ostream& operator<<(std::ostream& os, glm::vec3 val);


#endif
