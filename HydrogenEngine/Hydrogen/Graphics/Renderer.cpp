#include "Renderer.h"
#include <time.h>

std::vector<Hydrogen::HydrogenRenderCommand> FilterBlending(std::vector<Hydrogen::HydrogenRenderCommand> commands, Hydrogen::HydrogenBlendingInfo blending) {
	std::vector<Hydrogen::HydrogenRenderCommand>Filtered;
	for (auto iterator = commands.begin(); iterator != commands.end(); iterator++) {
		if (iterator->Model->GetComponents()->BlendingInfo == blending) {
			Filtered.push_back(*iterator);
		}
	}
	return Filtered;
}

void SortByDistance(std::vector<Hydrogen::HydrogenRenderCommand>& commands, Hydrogen::HydrogenCamera* camera) {
	std::sort(commands.begin(), commands.end(), [camera](Hydrogen::HydrogenRenderCommand first, Hydrogen::HydrogenRenderCommand second)->bool {
		glm::vec3 pos = camera->GetPosition();
		float d0 = glm::distance(pos, first.Model->GetTransform()->GetTranslation());
		float d1 = glm::distance(pos, second.Model->GetTransform()->GetTranslation());
		return (d0 > d1);
		}
	);
}

namespace Hydrogen {

	const static unsigned int ForwardDrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	const static float DefaultColor[4]{ 0.0f };
	const static float DefaultDepth[1]{ 1.0f };

	double GetNanos(void) {
		_timespec64 time;
		_timespec64_get(&time, TIME_UTC);
		long long raw = time.tv_nsec + time.tv_sec * 1000000000;
		return double(raw) / 1000000000.0;
	}

	struct HydrogenDebugOutputStream {
		std::ostringstream Stream;
		~HydrogenDebugOutputStream(void) {
			std::cout << Stream.str();
		}
	} HydrogenOutputStream;

	struct HydrogneDebugTimer {
		double Start;
		void Begin(void) {

			Start = GetNanos();
		}
		void End(void) {
			double delta = GetNanos() - Start;
			double ops = 1.0f / delta;
			HydrogenOutputStream.Stream << ops << " operations per second\n";
		}
	};
	 
	bool LoadedOpenGL = false;
	void LoadOpenGL(void) {
		if (!LoadedOpenGL) {
			glewInit();
			LoadedOpenGL = true; 
			return; 
			glCall(glEnable(GL_DEPTH_TEST));
			glCall(glEnable(GL_BLEND));
			glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
			glCall(glEnable(GL_CULL_FACE));
			glCall(glCullFace(GL_FRONT));
			glCall(glFrontFace(GL_CCW));
		}
	}

	void HydrogenRenderer::WindowEventProcdure(const HydrogenWindowEvent& event, const std::pmr::vector<char>& data) {
		///Returning to my code about a month later, it seems as if it would've been a better idea to access the vector's data instead of just doing a brute force cast
		switch(event.Type) {
			case HYDROGEN_WINDOW_EVENT_TYPE_FRAMEBUFFER_RESIZE: {
				std::pmr::vector<int>& read = *(std::pmr::vector<int>*)&data;
				int x = read[event.Data];
				int y = read[event.Data + 1];
				glCall(glViewport(0, 0, x, y));
				Camera->SetAspectRatio((float)x / float(y));
			} break;
			default: {
				std::stringstream ExceptionMessageBuilder("Unrecognized event type: ");
				ExceptionMessageBuilder << event.Type;
				ThrowException(ExceptionMessageBuilder.str());
			} break;
		}
		
	}

	HydrogenRenderer::HydrogenRenderer(const HydrogenWindow& window) : EventKey(window.GetEventKey()) {
		LoadOpenGL(); 
		Shaders.PostProcessing.HighDynamicRange.CreateIdentifier();
		Shaders.PostProcessing.HighDynamicRange.CompileShaderFiles("res/shaders/PostProcessing/HighDynamicRange/VertexShader.glsl", "res/shaders/PostProcessing/HighDynamicRange/FragmentShader.glsl");
		Shaders.PhysicallyBased.CreateIdentifier();
		Shaders.PhysicallyBased.CompileShaderFiles("res/shaders/PhysicallyBased/VertexShader.glsl", "res/shaders/PhysicallyBased/FragmentShader.glsl");
		Shaders.Skybox.CreateIdentifier();
		Shaders.Skybox.CompileShaderFiles("res/shaders/Skybox/VertexShader.glsl", "res/shaders/Skybox/FragmentShader.glsl");
		const std::vector<HydrogenVertex> CubeVertices = {
			HydrogenVertex(glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0, 1, 0), glm::vec2(2.5, 4), glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1)),
			HydrogenVertex(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0, 1, 0), glm::vec2(1.5, 3), glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1)),
			HydrogenVertex(glm::vec3(0.5, 0.5, -0.5), glm::vec3(0, 1, 0), glm::vec2(1.5, 4), glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1)),
			HydrogenVertex(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0, 0, 1), glm::vec2(2.5, 3), glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0)),
			HydrogenVertex(glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0, 0, 1), glm::vec2(1.5, 2), glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0)),
			HydrogenVertex(glm::vec3(0.5, -0.5, 0.5), glm::vec3(0, 0, 1), glm::vec2(1.5, 3), glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0)),
			HydrogenVertex(glm::vec3(-0.5, 0.5, 0.5), glm::vec3(-1, 0, 0), glm::vec2(2.5, 2), glm::vec3(0, 1, 0), glm::vec3(0, 0, -1)),
			HydrogenVertex(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(-1, 0, 0), glm::vec2(1.5, 1), glm::vec3(0, 1, 0), glm::vec3(0, 0, -1)),
			HydrogenVertex(glm::vec3(-0.5, -0.5, 0.5), glm::vec3(-1, 0, 0), glm::vec2(1.5, 2), glm::vec3(0, 1, 0), glm::vec3(0, 0, -1)),
			HydrogenVertex(glm::vec3(0.5, -0.5, -0.5), glm::vec3(0, -1, 0), glm::vec2(2.5, 1), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1)),
			HydrogenVertex(glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0, -1, 0), glm::vec2(1.5, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1)),
			HydrogenVertex(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0, -1, 0), glm::vec2(1.5, 1), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1)),
			HydrogenVertex(glm::vec3(0.5, 0.5, -0.5), glm::vec3(1, 0, 0), glm::vec2(1.5, 2), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
			HydrogenVertex(glm::vec3(0.5, -0.5, 0.5), glm::vec3(1, 0, 0), glm::vec2(0.5, 1), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
			HydrogenVertex(glm::vec3(0.5, -0.5, -0.5), glm::vec3(1, 0, 0), glm::vec2(0.5, 2), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
			HydrogenVertex(glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(3.5, 2), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0)),
			HydrogenVertex(glm::vec3(0.5, -0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(2.5, 1), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0)),
			HydrogenVertex(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(2.5, 2), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0)),
			HydrogenVertex(glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0, 1, 0), glm::vec2(2.5, 4), glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1)),
			HydrogenVertex(glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0, 1, 0), glm::vec2(2.5, 3), glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1)),
			HydrogenVertex(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0, 1, 0), glm::vec2(1.5, 3), glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1)),
			HydrogenVertex(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0, 0, 1), glm::vec2(2.5, 3), glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0)),
			HydrogenVertex(glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0, 0, 1), glm::vec2(2.5, 2), glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0)),
			HydrogenVertex(glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0, 0, 1), glm::vec2(1.5, 2), glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0)),
			HydrogenVertex(glm::vec3(-0.5, 0.5, 0.5), glm::vec3(-1, 0, 0), glm::vec2(2.5, 2), glm::vec3(0, 1, 0), glm::vec3(0, 0, -1)),
			HydrogenVertex(glm::vec3(-0.5, 0.5, -0.5), glm::vec3(-1, 0, 0), glm::vec2(2.5, 1), glm::vec3(0, 1, 0), glm::vec3(0, 0, -1)),
			HydrogenVertex(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(-1, 0, 0), glm::vec2(1.5, 1), glm::vec3(0, 1, 0), glm::vec3(0, 0, -1)),
			HydrogenVertex(glm::vec3(0.5, -0.5, -0.5), glm::vec3(0, -1, 0), glm::vec2(2.5, 1), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1)),
			HydrogenVertex(glm::vec3(0.5, -0.5, 0.5), glm::vec3(0, -1, 0), glm::vec2(2.5, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1)),
			HydrogenVertex(glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0, -1, 0), glm::vec2(1.5, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1)),
			HydrogenVertex(glm::vec3(0.5, 0.5, -0.5), glm::vec3(1, 0, 0), glm::vec2(1.5, 2), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
			HydrogenVertex(glm::vec3(0.5, 0.5, 0.5), glm::vec3(1, 0, 0), glm::vec2(1.5, 1), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
			HydrogenVertex(glm::vec3(0.5, -0.5, 0.5), glm::vec3(1, 0, 0), glm::vec2(0.5, 1), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
			HydrogenVertex(glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(3.5, 2), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0)),
			HydrogenVertex(glm::vec3(0.5, 0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(3.5, 1), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0)),
			HydrogenVertex(glm::vec3(0.5, -0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(2.5, 1), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0))
		};
		const std::vector<unsigned int> CubeElements = {
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35
		};
		Shapes.Cube.CreateIdentifier();
		Shapes.Cube.UploadData(CubeVertices, CubeElements);
		glCall(glCullFace(GL_BACK));
		glCall(glFrontFace(GL_CCW));
		glCall(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS));
		Framebuffers.Forward.Framebuffer.CreateIdentifier();
		Framebuffers.Forward.Color.CreateIdentifier();
		Framebuffers.Forward.Depth.CreateIdentifier();
		Framebuffers.Forward.Color.CreateEmpty(HYDROGEN_TEXTURE_INTERNAL_FORMAT_RGBA_8, 1280, 720);
		Framebuffers.Forward.Depth.CreateEmpty(HYDROGEN_TEXTURE_INTERNAL_FORMAT_DEPTH_16, 1280, 720);
		Framebuffers.Forward.Color.FilteringMode(HYDROGEN_TEXTURE_FILTERING_MODE_NEAREST, HYDROGEN_TEXTURE_FILTERING_MODE_NEAREST);
		Framebuffers.Forward.Depth.FilteringMode(HYDROGEN_TEXTURE_FILTERING_MODE_NEAREST, HYDROGEN_TEXTURE_FILTERING_MODE_NEAREST);
		Framebuffers.Forward.Framebuffer.AttachTexture(HYDROGEN_TEXTURE_TARGET_TEXTURE_2D, HYDROGEN_FRAMEBUFFER_ATTACHMENT_COLOR_0, &Framebuffers.Forward.Color);
		Framebuffers.Forward.Framebuffer.AttachTexture(HYDROGEN_TEXTURE_TARGET_TEXTURE_2D, HYDROGEN_FRAMEBUFFER_ATTACHMENT_DEPTH, &Framebuffers.Forward.Depth);
		Shapes.Quad.CreateIdentifier();
		std::vector<HydrogenVertex> QuadVertices = {
			HydrogenVertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
			HydrogenVertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
			HydrogenVertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
			HydrogenVertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
		};
		std::vector<unsigned int> QuadIndicies = {
			0, 2, 3, 2, 0, 1
		};
		Shapes.Quad.UploadData(QuadVertices, QuadIndicies);
	}

	HydrogenRenderer::~HydrogenRenderer(void) { 
		Shaders.PostProcessing.HighDynamicRange.DeleteIdentifier();
		Shaders.PhysicallyBased.DeleteIdentifier();
		Shaders.Skybox.DeleteIdentifier();
		Environment->LightBuffer.DeleteIdentifier();
	}

	HydrogenFrame* HydrogenRenderer::NewFrame(void) {
		Framebuffers.Forward.Framebuffer.Bind();
		glCall(glDrawBuffers(1, ForwardDrawBuffers));
		glCall(glClearBufferfv(GL_COLOR, 0, DefaultColor));
		glCall(glClearBufferfv(GL_DEPTH, 0, DefaultDepth));
		Framebuffers.Forward.Framebuffer.Unbind();
		return &CurrentFrame;
	}
	void HydrogenRenderer::EndFrame(void) {
		FlushRenderCommands();
		//Now do post processing
		glCall(glDisable(GL_DEPTH_TEST));
		glCall(glDisable(GL_BLEND));
		glCall(glDisable(GL_CULL_FACE));
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		Shaders.PostProcessing.HighDynamicRange.UseShader();
		Shaders.PostProcessing.HighDynamicRange.UploadFloat("Exposure", Camera->Exposure);
		Shaders.PostProcessing.HighDynamicRange.UploadFloat("Contrast", Camera->Contrast / 2.2f);
		glCall(glActiveTexture(GL_TEXTURE0));
		Framebuffers.Forward.Color.Bind();
		Shaders.PostProcessing.HighDynamicRange.UploadTexture("ColorBuffer", 0);
		Shapes.Quad.Bind();
		glCall(glDrawElements(GL_TRIANGLES, Shapes.Quad.GetElementCount(), GL_UNSIGNED_INT, 0));
		Shapes.Quad.Unbind();
	}

	void HydrogenRenderer::ProcessEvents(void) {
		HydrogenWindowEventAllocator& EventAllocator = *HydrogenEventAllocatorMap[EventKey];
		std::pmr::vector<int>& read = *(std::pmr::vector<int>*) & EventAllocator.EventAllocator;
		for (std::multimap<GLFWwindow*, HydrogenWindowEvent>::iterator WindowEvent = HydrogenEventStorage.begin(); WindowEvent != HydrogenEventStorage.end(); WindowEvent++) {
			if (WindowEvent->first == EventKey) {
				WindowEventProcdure(WindowEvent->second, EventAllocator.EventAllocator);
				HydrogenEventStorage.erase(WindowEvent);
			}
		}
		EventAllocator.EventAllocator.resize(HYDROGEN_WINDOW_EVENT_ALLOCATOR_DEFAULT_SIZE);
		std::pmr::vector<char>(EventAllocator.EventAllocator).swap(EventAllocator.EventAllocator);
		EventAllocator.AllocationIndex = 0;
	}

	void HydrogenRenderer::RegisterCamera(const HydrogenCamera& camera) { 
		Camera = (HydrogenCamera*)&camera;
	}

	void HydrogenRenderer::PushRenderCommand(HydrogenModel* object) {
		HydrogenRenderCommand command;
		command.Model = object;
		DrawCommands.push_back(command);
	}


	//THIS FUNCTION IS ONLY FOR RENDERING THE SCENE, NO POST PROCESSING
	void HydrogenRenderer::FlushRenderCommands(void) {
		/*Say hello to the most longest function I will ever probably write in my life
		I might split stuff like the shadow maps outside of this function so flushing the draw commands basically has nothing to do with IBL rendering or shadow maps
		Anyway let's start with a basic overview of this function:
		- We first need to build th rendering list. This contains what should be rendered, what order, etc
		- Then we do a bunch of framebuffer stuff, and then start rendering. Shaders will be a part of the renderer
		- Once we are done rendering the scene, we do post processing effects
		- Finally once we are done we wrap everything up for a buffer swap
		*/
		//Alright let's start with rendering list. We sort by transparency first. The reality is, deffered rendering supports transparency, but not translucency 
		//1. TODO: View frustrum culling
		//2. Sort objects by opaque and translucency and trasparency
		std::vector<HydrogenRenderCommand> OpaqueCommands = FilterBlending(DrawCommands, HYDROGEN_BLENDING_INFO_OPAQUE);
		std::vector<HydrogenRenderCommand> TransparentCommands = FilterBlending(DrawCommands, HYDROGEN_BLENDING_INFO_TRANSPARENT);
		std::vector<HydrogenRenderCommand> TranslucentCommands = FilterBlending(DrawCommands, HYDROGEN_BLENDING_INFO_TRANSLUCENT);
		TransparentCommands.insert(TransparentCommands.begin(), TranslucentCommands.begin(), TranslucentCommands.end()); //Since right now I am not going for a too complex solution
		//3. TODO: Sort by distance 
		SortByDistance(OpaqueCommands, Camera);
		SortByDistance(TransparentCommands, Camera);
		//Build the light list and buffers
		auto lights = Environment->BuildPointLights();
		HydrogenMemoryBuffer buffer;
		for (auto light = lights.begin(); light != lights.end(); light++) {
			HydrogenPointLight* pointlight = (HydrogenPointLight*)*light;
			buffer << pointlight->Luminosity << pointlight->Position;
		}
		int fillerLights = 64 - lights.size(); //Engine currently supports 64 lights
		for (int counter = 0; counter < fillerLights; counter++) {
			buffer << glm::vec3(0.0f) << glm::vec3(0.0f);
		}
		buffer << (int)lights.size();
		Environment->LightBuffer.UploadData(&buffer);
		//Alright, now we need to prepare for drawing anything that's opaque. We need shaders for that
		glCall(glEnable(GL_DEPTH_TEST));
		glCall(glEnable(GL_BLEND));
		glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		Framebuffers.Forward.Framebuffer.Bind();
		for (auto model = OpaqueCommands.begin(); model != OpaqueCommands.end(); model++) {
			auto Components = model->Model->GetComponents();
			Shaders.PhysicallyBased.UseShader();
			//Now let's upload the transformations and other stuff necessary for rendering
			Shaders.PhysicallyBased.UploadTransform("Transform", Components->Transform);
			Shaders.PhysicallyBased.UploadCamera("Camera", *Camera);
			//Suggested texture model: nessacry textures (diffuse, specular, metalness, smoothness, ao, edge, etc, go first in texture slots, then are the things like normal map, height map etc
			model->Model->ActivateTextureUnits();
			Shaders.PhysicallyBased.UploadTexture("DiffuseMap", 0);
			Shaders.PhysicallyBased.UploadTexture("NormalMap", 1);
			Shaders.PhysicallyBased.UploadTexture("HeightMap", 2);
			Shaders.PhysicallyBased.UploadTexture("MetallicMap", 3);
			Shaders.PhysicallyBased.UploadTexture("RoughnessMap", 4);
			Shaders.PhysicallyBased.UploadTexture("AmbientOcclusionMap", 5);
			//Now we upload lighting info to the shader. I would rather just create a buffer for lights altoghter for point lights 
			Shaders.PhysicallyBased.UploadBuffer("LightBuffer", &Environment->LightBuffer);
			//Acutally not now I have to make sure things work first
			Components->Mesh.Bind();
			glCall(glDrawElements(GL_TRIANGLES, Components->Mesh.GetElementCount(), GL_UNSIGNED_INT, 0));
			Components->Mesh.Unbind(); //Not really nessacry and slow but I'll keep it for now
		}
		for (auto model = TransparentCommands.begin(); model != TransparentCommands.end(); model++) {
			auto Components = model->Model->GetComponents();
			Shaders.PhysicallyBased.UseShader();
			//Now let's upload the transformations and other stuff necessary for rendering
			Shaders.PhysicallyBased.UploadTransform("Transform", Components->Transform);
			Shaders.PhysicallyBased.UploadCamera("Camera", *Camera);
			//Suggested texture model: nessacry textures (diffuse, specular, metalness, smoothness, ao, edge, etc, go first in texture slots, then are the things like normal map, height map etc
			model->Model->ActivateTextureUnits();
			Shaders.PhysicallyBased.UploadTexture("DiffuseMap", 0);
			Shaders.PhysicallyBased.UploadTexture("NormalMap", 1);
			Shaders.PhysicallyBased.UploadTexture("HeightMap", 2);
			Shaders.PhysicallyBased.UploadTexture("MetallicMap", 3);
			Shaders.PhysicallyBased.UploadTexture("RoughnessMap", 4);
			Shaders.PhysicallyBased.UploadTexture("AmbientOcclusionMap", 5);
			//Now we upload lighting info to the shader. I would rather just create a buffer for lights altoghter for point lights 
			Shaders.PhysicallyBased.UploadBuffer("LightBuffer", &Environment->LightBuffer);
			//Acutally not now I have to make sure things work first
			Components->Mesh.Bind();
			glCall(glDrawElements(GL_TRIANGLES, Components->Mesh.GetElementCount(), GL_UNSIGNED_INT, 0));
			Components->Mesh.Unbind(); //Not really nessacry and slow but I'll keep it for now
		}
		glCall(glDisable(GL_DEPTH_TEST));
		Framebuffers.Forward.Framebuffer.Unbind();
		DrawCommands.clear(); //So the next frame doesn't have to process the previous frame
		DrawCommands.shrink_to_fit(); //To prevent "memory leaks", basically draw calls fill up the vector and don't get released, although a more "intelligent" system would be appreicated 
	}

	
	void HydrogenRenderer::RegisterEnvironment(HydrogenEnvironment* environment) {
		Environment = environment;
		Environment->LightBuffer.CreateIdentifier();
		Environment->LightBuffer.ConfigureTarget(HYDROGEN_BUFFER_TARGET_UNIFORM);
	}

	void HydrogenRenderer::RenderSkybox(HydrogenSkybox* skybox) {
		Framebuffers.Forward.Framebuffer.Bind();
		glCall(glDisable(GL_CULL_FACE));
		glCall(glDepthMask(GL_FALSE));
		glCall(glDisable(GL_BLEND));
		Shaders.Skybox.UseShader();
		Shaders.Skybox.UploadCamera("Camera", *Camera);
		skybox->ActivateTextureUnits();
		Shaders.Skybox.UploadTexture("Skybox", 0);
		Shapes.Cube.Bind();
		glCall(glDrawElements(GL_TRIANGLES, Shapes.Cube.GetElementCount(), GL_UNSIGNED_INT, 0));
		Shapes.Cube.Unbind();
		glCall(glDepthMask(GL_TRUE));
		glCall(glEnable(GL_BLEND));
		glCall(glEnable(GL_CULL_FACE));
		Framebuffers.Forward.Framebuffer.Unbind();
	}

}