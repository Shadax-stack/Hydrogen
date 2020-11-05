#ifndef HYDROGEN_WINDOW_H
#define HYDROGEN_WINDOW_H

#include "../Core/HydrogenEngine.h"
#include <map>
#include <vector>
#include <memory_resource>

//Maybe I should create a class specifically for input instead of making the user do it themselves

namespace Hydrogen {

	#define HYDROGEN_WINDOW_EVENT_TYPE_FRAMEBUFFER_RESIZE 0
	struct HydrogenWindowEvent {
		unsigned int Type;
		unsigned int Data;
	};

#define HYDROGEN_WINDOW_EVENT_ALLOCATOR_DEFAULT_SIZE 4096
#define HYDROGEN_WINDOW_EVENT_ALLOCATOR_EFFICIENT_MEMORY_USAGE
	struct HydrogenWindowEventAllocator {
		std::pmr::monotonic_buffer_resource EventMemory;
		std::pmr::vector<char> EventAllocator;
		unsigned int AllocationIndex;
		HydrogenWindowEventAllocator(void);
		~HydrogenWindowEventAllocator(void) = default;
		unsigned int Allocate(std::size_t bytes);
		template<typename T> unsigned int Allocate(std::size_t count);
	};

	extern std::multimap<GLFWwindow*, HydrogenWindowEvent> HydrogenEventStorage;
	extern std::map<GLFWwindow*, HydrogenWindowEventAllocator*> HydrogenEventAllocatorMap;

	//TODO: Move things like input into another class
	class HYDROGEN_API HydrogenWindow {
	private:
		HydrogenWindowEventAllocator WindowEventAllocator;
		GLFWwindow* Window;
		float LastSwap;
		float DeltaTime;
	public:
		HydrogenWindow(unsigned int height, unsigned int width, const std::string & title);
		HydrogenWindow(unsigned int height, unsigned int width, const char* title);
		~HydrogenWindow(void);
		void SetCurrentContext(void); //I don't really like the function name, maybe ThreadCurrentContext?
		bool ShouldClose(void);
		static void PollEvents(void);
		void SwapBuffers(void);
		bool KeyPressed(unsigned int key);
		GLFWwindow* GetEventKey(void) const; //RetrieveEventKey
		glm::vec2 GetCursorPos(void) const;
		float GetDeltaTime(void);
	};

}

#endif

