#include "Window.h"
#include <vector>

namespace Hydrogen {

	//TODO: Replace this with EA's/Google's multimap for better performance
	std::multimap<GLFWwindow*, HydrogenWindowEvent> HydrogenEventStorage;

	/******************************************************************
	TODO: Find a way to get rid of the heap allocated memory each time I need to create a window event
	One way to do this is to go an use the old huge fixed-size array for my window event information
	But that's a really bad solution as you often have way too much memory
	The better solution is a highly efficent memory pool that will cut down the allocation / deallocation times in half
	Or I can make a memory pool specific for this where it clears after every frame, which is probably a better idea
	For such task having a std::vector<char> is a good idea since I can auto-resize if it overflows and use reserve for the memory pool part
	I will use char because it's the closest thing to a byte-type in C++
	For certain window events, I can make them use even less memory, such as the framebuffer resize event can use shorts instead of ints
	Basically each window event instead of pointing to some random location in the heap they will point to a position in the vector
	The way I will integrate this is swap out the void* for a unsigned integer index. The reason I will not use a pointer is because if the vector has to resize I have got lots of dangling pointers
	I am not using an iterator since:
	- I am not iterating over the vector
	- To make this program more simple
	*******************************************************************/
	//std::vector<char> HydrogenWindowEventMemoryAllocator(4096); //Initalize with 4096 bytes of memory
	//std::vector<char>& WindowEventMemoryAllocator = HydrogenWindowEventMemoryAllocator, MemoryAllocator = WindowEventMemoryAllocator; //To shorten the name
	//unsigned int HydrogenEventMemoryIndex = 0; //Increment as more memory gets used
	//I'll need to finish this up later. I cannot mainly do this now as I need to find out an efficenet way to deallocate memory assuming that there are multiple windows. I could iterate and check but that stops the "clear after each frame". Another thing I could do is have an allocator member variable but the callback function is static which would mean I would need a second map to figure out which vecotr to allocate from. I just want an easy and quick way to allocate and deallocate memory here.

	// 		EventAllocator = std::move(std::pmr::vector<char>(&EventMemory));

	HydrogenWindowEventAllocator::HydrogenWindowEventAllocator(void) : EventMemory(HYDROGEN_WINDOW_EVENT_ALLOCATOR_DEFAULT_SIZE), EventAllocator(/*&EventMemory*/), AllocationIndex(0) { //TODO: Get the PMR stiff working
																																														  //If I can't get that working, I'll just go back to regular old vectors
		EventAllocator = std::pmr::vector<char>(&EventMemory);
	}

	unsigned int HydrogenWindowEventAllocator::Allocate(std::size_t bytes) {
		unsigned int memory = AllocationIndex;
		AllocationIndex += bytes;
		EventAllocator.reserve(AllocationIndex);
		return memory;
	}

	template<typename T>
	unsigned int HydrogenWindowEventAllocator::Allocate(std::size_t count) {
		//A bunch of code to align AllocationIndex to size of T
		#ifdef HYDROGEN_WINDOW_EVENT_ALLOCATOR_EFFICIENT_MEMORY_USAGE
		unsigned int InitailIndex = AllocationIndex;
		#endif
		AllocationIndex = sizeof(T) * (AllocationIndex / sizeof(T));
		#ifdef HYDROGEN_WINDOW_EVENT_ALLOCATOR_EFFICIENT_MEMORY_USAGE
		if (InitailIndex != AllocationIndex) { //For faster calculations, use if(!(AllocationIndex - InitailIndex)) { [...] }. I am not for easier to read code
			AllocationIndex += sizeof(T);
		}
		#else
		AllocationIndex += sizeof(T);
		#endif
		unsigned int memory = AllocationIndex;
		AllocationIndex += count * sizeof(T);
		//Add a number (like 2048) for "breathing room" for the vector so you have fewer reallocations and stuff
		EventAllocator.reserve(AllocationIndex);
		EventAllocator.resize(AllocationIndex);
		return memory;
	}

	std::map<GLFWwindow*, HydrogenWindowEventAllocator*> HydrogenEventAllocatorMap;

	static void WindowEventCallback(GLFWwindow* window, int x, int y) { 
		std::cout << "Resize\n";
		HydrogenEventStorage.emplace(
			std::make_pair(
				window,
				HydrogenWindowEvent{ HYDROGEN_WINDOW_EVENT_TYPE_FRAMEBUFFER_RESIZE, [window, x, y]() {
						HydrogenWindowEventAllocator* Allocator = HydrogenEventAllocatorMap[window];
						unsigned int data = Allocator->Allocate<int>(2) / sizeof(int);
						std::pmr::vector<int>& write = *(std::pmr::vector<int>*)&Allocator->EventAllocator; //How to force a cast: OutputType output = *(OutputType*)&input;
						write[data] = x;
						write[data + 1] = y;
						return data;
					}() 
				}
			)
		);
	}

	struct HydrogenWindowInitailizer {
		HydrogenWindowInitailizer(void) {}
		~HydrogenWindowInitailizer(void) = default;
	} WindowStaticInitailizer;

	HydrogenWindow::HydrogenWindow(unsigned int width, unsigned int height, const std::string& title) : HydrogenWindow(width, height, title.c_str()) {} //Construct a window object using the c-string instead

	HydrogenWindow::HydrogenWindow(unsigned int width, unsigned int height, const char* title) : 
		Window(glfwCreateWindow(width, height, title, nullptr, nullptr)),
		WindowEventAllocator(),
		LastSwap(glfwGetTime()),
		DeltaTime(0.0f) 
	{ //Construct the window using a create window call
		HydrogenEventAllocatorMap.emplace(std::make_pair(Window, &WindowEventAllocator));
		glfwSetFramebufferSizeCallback(Window, WindowEventCallback); 
		glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		WindowEventCallback(Window, width, height); 
	} 

	HydrogenWindow::~HydrogenWindow(void) {
		glfwDestroyWindow(Window);
	}

	void HydrogenWindow::SetCurrentContext(void) {
		glfwMakeContextCurrent(Window);
	} 

	bool HydrogenWindow::ShouldClose(void) {
		return glfwWindowShouldClose(Window);
	} 

	void HydrogenWindow::PollEvents(void) {
		glfwPollEvents();
	} 

	void HydrogenWindow::SwapBuffers(void) {
		float CurrentTime = glfwGetTime();
		DeltaTime = CurrentTime - LastSwap;
		LastSwap = CurrentTime;
		glfwSwapBuffers(Window); 
	} 

	bool HydrogenWindow::KeyPressed(unsigned int key) { 
		return glfwGetKey(Window, key);
	}

	GLFWwindow* HydrogenWindow::GetEventKey(void) const { 
		return Window;
	}

	glm::vec2 HydrogenWindow::GetCursorPos(void) const {
		double x, y;
		glfwGetCursorPos(Window, &x, &y);
		return glm::vec2(x, y);
	}

	float HydrogenWindow::GetDeltaTime(void) {
		return DeltaTime;
	}

}