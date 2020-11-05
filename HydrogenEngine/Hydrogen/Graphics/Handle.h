#ifndef HYDROGEN_HANDLE_H
#define HYDROGEN_HANDLE_H

#include "../Core/HydrogenEngine.h"

namespace Hydrogen {

	struct HYDROGEN_API HydrogenHandleInfo {

	};

	//The superclass for all OpenGL object wrappers
	class HYDROGEN_API HydrogenHandle {
	protected:
		unsigned int Handle;
	public:
		HydrogenHandle(void);
		~HydrogenHandle(void) = default;
		//I'm not making creating and deleting handles virtual, it's highly unlikely that you would need to create/destroy a generic object
		void CreateIdentifier(void);
		void DeleteIdentifier(void);
		void SetIdentifier(unsigned int id);
		//But then again identifier creation is probably a slow process so overhead of virtual functions is insiginifacnt. My biggest concern is memory usage at that point
		unsigned int RetrieveIdentifier(void);
		unsigned int GetIdentifier(void);
		//Bind and unbind
		void Bind(void) = delete;
		static void Unbind(void) = delete;
	};

}

#endif
