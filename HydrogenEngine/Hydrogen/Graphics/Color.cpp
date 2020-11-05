#include "Color.h"

namespace Hydrogen {

	HydrogenColorRGBA::HydrogenColorRGBA(float red, float green, float blue, float alpha) : r(red), g(green), b(blue), a(alpha) {}

	HydrogenColorRGBA::HydrogenColorRGBA(float color) : HydrogenColorRGBA(color, color, color, color) {}

	HydrogenColorRGBA::HydrogenColorRGBA(void) : HydrogenColorRGBA(0.0f) {}



}