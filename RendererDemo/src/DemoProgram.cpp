#include <Hydrogen/Core/HydrogenEngine.h>
#include <Hydrogen/Graphics/Window.h>
#include <Hydrogen/Graphics/Renderer.h>
#include <Hydrogen/Graphics/Mesh.h>
#include <Hydrogen/Graphics/Shader.h>
#include <Hydrogen/Graphics/Texture2D.h>
#include <Hydrogen/Graphics/Framebuffer.h>
#include <Hydrogen/Graphics/Object.h>
#include <Hydrogen/Math/LinearAlgebra.h>
#include <Hydrogen/Graphics/Model.h>
#include <Hydrogen/Graphics/Skybox.h>
#include <iostream>
#include <algorithm>

glm::vec2 LastPos;
const float Sensitivity = 0.0015f;

void RotateCamera(Hydrogen::HydrogenCamera& camera, const Hydrogen::HydrogenWindow& window){
	glm::vec2 CurrentPos = window.GetCursorPos();
	glm::vec2 DeltaPos(CurrentPos.x - LastPos.x, LastPos.y - CurrentPos.y);
	camera.IncreaseRotation(glm::vec3(DeltaPos * Sensitivity, 0.0f));
	LastPos = CurrentPos;
}

const float Speed = 5.0f;

int main() {
	try {
		Hydrogen::HydrogenWindow Window(1280, 720, "OpenGL");
		Window.SetCurrentContext();
		Hydrogen::HydrogenRenderer Renderer(Window);
		Hydrogen::HydrogenCamera Camera; {
			Renderer.RegisterCamera(Camera);
			Camera.Exposure = 0.707f; 
		}
		Hydrogen::HydrogenEnvironment Environment; {
			Hydrogen::HydrogenPointLight Light;
			Light.Luminosity = glm::vec3(1.0f);
			Light.Position = glm::vec3(0.0f, 2.0f, 0.0f);
			Environment.AddPointLight(Light);
			Light.Position = glm::vec3(0.0f, -2.0f, 0.0f);
			Environment.AddPointLight(Light);
			Renderer.RegisterEnvironment(&Environment);
		}
		Hydrogen::HydrogenModel Cube;
		Cube.LoadModel("res/models/Cube2.txt");
		Cube.GetComponents()->Transform.SetUVCoordMult(glm::vec2(4.0f));
		Hydrogen::HydrogenSkybox Skybox;
		Skybox.LoadSkybox("res/skyboxes/skybox1.txt");
		LastPos = Window.GetCursorPos();
		while (!Window.ShouldClose()) {
			Renderer.NewFrame();
			RotateCamera(Camera, Window);
			glm::vec3 Direction = Camera.GetDirection() * Speed;
			if (Window.KeyPressed('W')) Camera.IncreasePosition( Direction * Window.GetDeltaTime());
			if (Window.KeyPressed('S')) Camera.IncreasePosition(-Direction * Window.GetDeltaTime());
			Renderer.RenderSkybox(&Skybox);
			Renderer.PushRenderCommand(&Cube);
			Renderer.EndFrame();
			Window.SwapBuffers();
			Window.PollEvents();
			Renderer.ProcessEvents();
		}
	}
	catch (const Hydrogen::Exception & exception) {
		std::cout << Hydrogen::ReadException(exception) << '\n';
		return -1;
	}
	return 0;
}