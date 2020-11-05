#include "HydrogenEngine.h"
#include <sstream>

namespace Hydrogen {

	HydrogenInstance::HydrogenInstance(void) {
		//Automatically load GLFW when the library is loaded
		glfwInit();
		//TODO: Somehow flip the textures like you can do in stb_image
	}

	HydrogenInstance::~HydrogenInstance(void) {
		//Automatically unload GLFW when the library is loaded
		glfwTerminate();
	}

	void HydrogenInstance::AddRenderer(void) {
		RendererCount++;
	}
	void HydrogenInstance::ReleaseRenderer(void) {
		RendererCount--;
	}
	
	int HydrogenInstance::GetRendererCount(void) {
		return RendererCount;
	}

	//Instnace of the library
	HydrogenInstance HydrogenGlobalInstance;

	//Contsruct the string object using the input error message
	HydrogenException::HydrogenException(const char* message) : Message(message) {}
	HydrogenException::HydrogenException(const std::string& message) : Message(message) {}

	//I made this message const so people can do catch(const Hydrogen::HydrogenException & e)
	std::string HydrogenException::GetMessage(void) const {
		return Message;
	}
	void CheckError(unsigned int line, const char* func, const char* file) {
		//OpenGL functions generate only one error, so glGetError returns the error created by the latest function call
		//So if the latest error is not GL_NO_ERROR, which expands to 0, we  will throw an exception.
		unsigned int error = glGetError(); 
		//We take advantage of the fact that GL_NO_ERROR is 0 to make optimizations where we get rid of any logical operators
		//This should improve performance a tiny bit in our applications when in debug mode
		if (error) {
			//Gets the number of digits in a number
			auto CountDigits = [](unsigned int value) {
				//Start at 0 digits
				int count = 0;
				//Loop to get the number of digits
				while (value) { //Is the number 0 yet?
					//Divide by 10
					//We will use integer rounding to determine the number of digits
					value /= 10;
					//Increase the digit counter
					++count;
				}
				return count;
			};
			//Fast algorithm to count the number of characters in a c-style string
			auto StringLength = [](const char* string) {
				int length = 0;
				//Keep on moving forward until you hit the null terminator
				while (string[++length]);
				return length;
			};
			//Create a preallocated string for the string stream
			auto CreatePreallocatedString = [](unsigned int length) {
				std::string prealloc;
				//Use the reserve function to create a preallocated string
				prealloc.reserve(length);
				return prealloc;
			};
			//In the future I want a Java-like excpetion format without having to do a bunch of stack-tracing
			//So basically the format is "error at func(file_without_long_path:line_number)"
			//Default length for the format
			const unsigned int FormatLength = StringLength("OpenGL Error") + StringLength(" in function ") + StringLength(" at line ") + StringLength(" in file ");
			//Create/build the error message using a stringstream                        error is always going to be a 4 digit number but for the sake of consistency I will count the digits anyway
			std::ostringstream message(std::move(CreatePreallocatedString(FormatLength + CountDigits(error) + CountDigits(line) + StringLength(file) + StringLength(func))));
			//Ideally what I would do is create my own custom class for this which I can do " << thing " which adds to the total intger length value and actually doesn't build the string
			//This is much more easier than my current solutions whith lamdas and stuff
			message << "OpenGL Error " <<
				[error]() -> const char* {
					switch (error)
					{
					case GL_INVALID_ENUM:
						return("GL_INVALID_ENUM");
						break;
					case GL_INVALID_VALUE:
						return("GL_INVALID_VALUE");
						break;
					case GL_INVALID_OPERATION:
						return("GL_INVALID_OPERATION");
						break;
					case GL_INVALID_FRAMEBUFFER_OPERATION:
						return("GL_INVALID_FRAMEBUFFER_OPERATION");
						break;
					case GL_OUT_OF_MEMORY:
						return("GL_OUT_OF_MEMORY");
						break;
					case GL_STACK_OVERFLOW:
						return("GL_STACK_OVERFLOW");
						break;
					case GL_STACK_UNDERFLOW:
						return("GL_STACK_OVERFLOW");
						break;
					default:
						return "(Unknown error)";
				}
				}() 
			<< " in function " << func << " at line " << line << " in file " << file;
			//Finally throw the exception
			ThrowException(message.str());
		}
	}

	//The 3 functions are basically there to allow switching between the custom HydrogenException and the standard C++ exception with a simple preprocessor define  
	void ThrowException(const std::string & message) {
		throw CreateException(message);
	}

	Exception CreateException(const std::string & message) {
		#ifdef STD_EXCEPTIONS
		//std::exception doesn't support std::string for some reason
		return Exception(message.c_str());
		#else
		return Exception(message);
		#endif
	}

	std::string ReadException(Exception exception) {
		#ifdef STD_EXCEPTIONS
		return exception.what();
		#else
		return exception.GetMessage();
		#endif
	}

}