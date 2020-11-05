/*************************************************************
One major thing I have to do is replace most of the lamdas I have in my code with utlity functions
For example, I should move code that reads a file into a string out of a lambda and into a utlity folder that contains code like that
Also I need to get rid of reliance on GLM for performance reasons
Another thing I need to do is integrate JSON into my code for multiple things.
For example, shader.json would contain the path/source code of the required shader stages and possibly other information
Another example, model.json could contain paths to the textures and vertex model used by the program
However, I will after that move away from JSON for a faster format
Also I will replace the uniforms we have now with a faster format so we can have more data. For example, we can stuff the camera in a UBO so we can store extra data like direction as well as the transformation matrices

TODO: Move away from creating everything in the constructor and have methods to do that instead. This should give me a lot of flexibilty and increased speed when consturcting (and possible destructing) handle objects
The handl class is the superclass for any opengl object wrapper
The methods to construct handle wrappers will take in a "struct" of constant parameters
For example, to create a 2D texture, it could take a parameters:
1. Is this a FBO attachment?
	a. What's the depth & stencil / color type?
	b. What's the data type?
2. Is this a normal texture?
	a. What texture filtering mode should be used?
	b Should we create mipmaps?
We will try to autmoatically fill in some of the paramters so this becomes OpenGL but you have structs
For example Linear filtering means Linear Min and Mag filters, and liner if mipmaping
We will simplyfy the color formats to a few options, and depth/stencil as D24S8 only.
**************************************************************/

#ifndef HYDROGEN_ENGINE_H
#define HYDROGEN_ENGINE_H

#ifdef HYDROGEN_BUILD
#define HYDROGEN_API _declspec(dllexport)
#else
#define HYDROGEN_API _declspec(dllimport)
#endif

#define STD_EXCEPTIONS //Use standard C++ exceptions instead of Hydrogen's custom exception class
#define HYDROGEN_DEBUG //Use Hydrogen's custom debug features such as debugging by OpenGL function call
#define HYDROGEN_TEXTURE_LOAD_SOIL2 //Use SOIL2 instead of SOIL for image loading

//TODO: Move these inclusions to an include directory
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifdef HYDROGEN_TEXTURE_LOAD_SOIL2
#include <SOIL2/SOIL2.h>
#else
#include <SOIL/SOIL.h>
#endif
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdio.h>
#include <expat.h>
#include <libxml/xinclude.h>

#include <iostream>
#include <string>
#include <sstream>
//#include <bits/stdc++.h> //TODO: Remove all other STD C++ inclusions in my code //TODO: Fix this error when including this file
//We assume that the program is being compiled with either ICC, GCC, or MSVC. I don't really know about clang, maybe someone else can add support for it or something
#ifdef __GNUC__
#include <bits/stdc++.h>
#else
//TODO: get this working
//#define _SILENCE_ALL_CXX17_C_HEADER_DEPRACATION_WARNINGS
//#include <__msvc_all_public_headers.hpp>
#endif

#ifdef STD_EXCEPTIONS
#include <exception>
#include <stdexcept>
#endif

#ifdef __GNUC__
#define __FUNCTION__ __func__
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

namespace Hydrogen {
	struct HYDROGEN_API HydrogenInstance {
	private:
		unsigned int RendererCount = 0;
	public:
		HydrogenInstance(void);
		~HydrogenInstance(void);
		void AddRenderer(void);
		void ReleaseRenderer(void);
		int GetRendererCount(void);
	};

	//The library's instnace 
	//This should contain useful informations (mainly for debug) like total number of Hydrogen objects, memory stats, etc
	extern HydrogenInstance HydrogenGlobalInstance;

	typedef std::string HydrogenString;

	class HYDROGEN_API HydrogenException {
	private:
		std::string Message; //Not the best variable name, I should get one that more acurately represents stack trace and stuff, which I need to add later
	public:
		HydrogenException(const char* message);
		HydrogenException(const std::string & message);
		std::string GetMessage(void) const; //Not the best function name, I'll have to find a different one
	};
	#ifdef STD_EXCEPTIONS
	typedef std::exception Exception;
	#else
	typedef HydrogenException Exception;
	#endif
	void HYDROGEN_API ThrowException(const std::string& message);
	Exception HYDROGEN_API CreateException(const std::string& message);
	std::string HYDROGEN_API ReadException(Exception e);
	void HYDROGEN_API CheckError(unsigned int line, const char* func, const char* file);
	#ifdef HYDROGEN_DEBUG
	#define glFunction(func) func; CheckError(__LINE__, __FUNCTION__, __FILE__ )
	#else
	#define glFunction(func) func
	#endif
	#define glCall glFunction
}
#endif