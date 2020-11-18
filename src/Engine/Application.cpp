//
//	Created by MarcasRealAccount on 17. Oct. 2020
//
//	TODOs for the future:
//	- Maybe have m_Renderer.Render() get executed from a render thread as opposed to the window thread.
//

#include "Engine/Application.h"
#include "Engine/Utility/Logger.h"
#include "Engine/Utility/Config/ConfigManager.h"
#include "Engine/Input/InputHandler.h"
#include "Engine/Input/JoystickHandler.h"
#include "Engine/Renderer/Shader/Shader.h"
#include "Engine/Scene/Entity.h"

#include "Engine/Renderer/Shader/Material.h"

#include "Engine/Audio/AudioCore.h"

#include "Engine/Renderer/Mesh/StaticMesh.h"
#include "Engine/Renderer/Mesh/Loaders/OBJLoader.h"

namespace gp1 {

	TestEntity::TestEntity()
		: m_Mesh(new StaticMesh()), m_Material(new Material()) {

		meshLoaders::LoadOBJFile(*m_Mesh, "Mesh.obj");

		m_Material->SetShader(Shader::GetShader("shader"));

		this->m_Position.z = -5.0f;
		this->m_Rotation.x = 0.3f;
		this->m_Rotation.y = 0.4f;
	}

	TestEntity::~TestEntity() {
		delete this->m_Mesh;
		delete this->m_Material;
	}

	Mesh* TestEntity::GetMesh() const {
		return this->m_Mesh;
	}

	Material* TestEntity::GetMaterial() const {
		return this->m_Material;
	}

	Application::Application() {
		Logger::Init();
		AudioCore::Init();
		m_Window.Init();
		input::InputHandler::m_Window = &m_Window;
		input::JoystickHandler::Init();
		m_Renderer = Renderer::GetRenderer(RendererType::OPENGL, &m_Window);
		m_Renderer->Init();

		this->m_Scene.AttachEntity(&this->m_TestEntity);
		this->m_Scene.AttachEntity(&this->m_Camera);
		this->m_Camera.SetAsMainCamera();
	}

	void Application::Run() {
		while (!m_Window.IsCloseRequested()) {
			m_Renderer->Render(&this->m_Scene);
			m_Window.OnUpdate();
			input::JoystickHandler::OnUpdate();
		}
	}

	Application::~Application() {
		this->m_Renderer->DeInit();
		delete m_Renderer;
		Shader::CleanUpShaders();
		m_Window.DeInit();
		input::JoystickHandler::DeInit();
		input::InputHandler::CleanUp();
		config::ConfigManager::SaveConfigs();
		AudioCore::Shutdown();
		Logger::DeInit();
	}

} // namespace gp1
