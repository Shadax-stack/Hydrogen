#ifndef HYDROGEN_RENDERER_H
#define HYDROGEN_RENDERER_H

#include "../Core/HydrogenEngine.h"
#include "Window.h"
#include "Shader.h"
#include "Model.h"
#include "CommandBuffer.h"
#include "Environment.h"
#include "../Math/Camera.h"
#include <vector>
#include "Skybox.h"
#include "Framebuffer.h"


namespace Hydrogen {

	void HYDROGEN_API LoadOpenGL(void);

	struct HydrogenFrame {

	};

	class HYDROGEN_API HydrogenRenderer {
	private:
		struct {
			HydrogenShader PhysicallyBased;
			HydrogenShader Skybox;
			struct {
				HydrogenShader HighDynamicRange;
			} PostProcessing;
		} Shaders;
		struct {
			HydrogenMesh Cube;
			HydrogenMesh Quad;
		} Shapes;
		struct {
			struct  {
				HydrogenFramebuffer Framebuffer; // Main output color buffer
				HydrogenTexture2D Color;
				HydrogenTexture2D Depth;
			} Forward;
		} Framebuffers;
		std::vector<HydrogenRenderCommand> DrawCommands;
		HydrogenFrame CurrentFrame;
		void WindowEventProcdure(const HydrogenWindowEvent& event, const std::pmr::vector<char>& data);
	public:
		GLFWwindow* EventKey;
		//I think the enviornment and camera should be part of the render/created by it instead of being something that the user has to register
		HydrogenCamera* Camera;
		HydrogenEnvironment* Environment;
		HydrogenRenderer(const HydrogenWindow & window);
		~HydrogenRenderer(void);
		HydrogenFrame* NewFrame(void);
		void EndFrame(void); 
		void ProcessEvents(void);
		void RegisterCamera(const HydrogenCamera& camera);
		void RegisterEnvironment(HydrogenEnvironment* environment);
		void PushRenderCommand(HydrogenModel* object);
		void FlushRenderCommands(void);
		void RenderSkybox(HydrogenSkybox* skybox);
	};


}

#endif