#ifndef HYDROGEN_COLOR_H
#define HYDROGEN_COLOR_H

#include "../Core/HydrogenEngine.h"

namespace Hydrogen {

	//TODO: Operator overalod the vectors
	//TODO: Add the version of this struct which uses bytes and shorts instead of floats
	struct HYDROGEN_API HydrogenColorRGBA { 
		//TODO: Somehow get the variable names red, green, blue, and alpah working for the struct without increase size
		//GLM did this via unions, I'll have to look into that.
		float r;
		float g;
		float b;
		float a;
		HydrogenColorRGBA(float red, float green, float blue, float alpha);
		HydrogenColorRGBA(float color);
		HydrogenColorRGBA(void);
		~HydrogenColorRGBA(void) = default;
	};

}

#endif
