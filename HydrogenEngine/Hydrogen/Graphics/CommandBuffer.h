#ifndef HYDROGEN_COMMAND_BUFFER_H
#define HYDROGEN_COMMAND_BUFFER_H

#include "../Core/HydrogenEngine.h"
#include "Model.h"

namespace Hydrogen {

	//The model basically contains everything we need for the command, I'm keeping it as a struct just in case we will ever need anything else
	struct HydrogenRenderCommand {
		HydrogenModel* Model;
	};

}

#endif