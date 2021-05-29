//
//	Created by MarcasRealAccount on 17. Oct. 2020
//

#pragma once

#include "Engine/Renderer/Renderer.h"
#include "Engine/Scene/Camera.h"
#include "Engine/Scene/RenderableEntity.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Utility/Core.h"
#include "Engine/Utility/SmartPointers/SmartPointers.h"
#include "Engine/Window/Window.h"

namespace gp1
{
	//----
	// TODO(MarcasRealAccount): Please remove this when some actual rendering will take place, as this is just a test entity.
	class TestEntity : public scene::RenderableEntity
	{
	public:
		TestEntity();

		virtual void Update(float deltaTime) override;

		inline virtual smart_pointers::shared_ptr<renderer::Mesh> GetMesh() const override
		{
			return m_Mesh;
		}

		inline virtual smart_pointers::shared_ptr<renderer::Material> GetMaterial() const override
		{
			return m_Material;
		}

	private:
		smart_pointers::shared_ptr<renderer::StaticMesh> m_Mesh;
		smart_pointers::shared_ptr<renderer::Material>   m_Material;
	};
	//----

	class Application
	{
	public:
		~Application();

		inline window::Window& GetWindow()
		{
			return m_Window;
		}

		inline const window::Window& GetWindow() const
		{
			return m_Window;
		}

		inline smart_pointers::shared_ptr<renderer::Renderer> GetRenderer() const
		{
			return m_Renderer;
		}

		// Run the application.
		void Run();

	protected:
		Application();

	public:
		// Create the application.
		static Application* CreateApplication();

		// Get the application instance.
		static Application* GetInstance();

	private:
		// TODO: create init function for application using WindowData
		window::Window m_Window { { 1280, 720, 1280, 720, "Cherno Community Game Project 1", window::WindowMode::WINDOWED, false } }; // The window of this application.

		scene::Scene                   m_Scene; // The scene to use for this application.
		smart_pointers::shared_ptr<scene::Camera> m_Camera;

		//----
		// TODO(MarcasRealAccount): Please remove this when some actual rendering will take place, as this is just a test entity.
		smart_pointers::shared_ptr<TestEntity> m_TestEntities[10000];
		//----

		smart_pointers::shared_ptr<renderer::Renderer> m_Renderer;

	private:
		static Application* s_Instance; // The static application instance.
	};

} // namespace gp1
