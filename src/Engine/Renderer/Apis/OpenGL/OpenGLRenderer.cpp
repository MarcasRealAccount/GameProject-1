//
//	Created by MarcasRealAccount on 13. May. 2021
//

#include "Engine/Utility/Core.h"

#ifdef RENDERER_OPENGL

#include "Engine/Application.h"
#include "Engine/Renderer/Apis/OpenGL/Material/OpenGLMaterial.h"
#include "Engine/Renderer/Apis/OpenGL/Material/OpenGLReservedUniformBuffers.h"
#include "Engine/Renderer/Apis/OpenGL/Material/OpenGLUniform.h"
#include "Engine/Renderer/Apis/OpenGL/Material/OpenGLUniformBuffer.h"
#include "Engine/Renderer/Apis/OpenGL/Mesh/OpenGLStaticMesh.h"
#include "Engine/Renderer/Apis/OpenGL/OpenGLDebugRenderer.h"
#include "Engine/Renderer/Apis/OpenGL/OpenGLRenderer.h"
#include "Engine/Renderer/Apis/OpenGL/Shader/OpenGLShaderProgram.h"
#include "Engine/Renderer/Apis/OpenGL/Texture/OpenGLTexture2D.h"
#include "Engine/Renderer/Apis/OpenGL/Texture/OpenGLTexture2DArray.h"
#include "Engine/Renderer/Apis/OpenGL/Texture/OpenGLTexture3D.h"
#include "Engine/Renderer/Apis/OpenGL/Texture/OpenGLTextureCubeMap.h"
#include "Engine/Scene/Camera.h"
#include "Engine/Utility/SmartPointers/SmartPointers.h"
#include "Engine/Window/Window.h"

#include <stdexcept>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace gp1::renderer::opengl
{
	bool OpenGLRenderer::IsCompatible() const
	{
		if (!glfwInit())
			return false;

		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		GLFWwindow* window = glfwCreateWindow(100, 100, "I'm a cool opengl window", nullptr, nullptr);

		glfwMakeContextCurrent(window);

		bool result = static_cast<bool>(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)));

		glfwDestroyWindow(window);

		return result && GLVersion.major >= 4 && GLVersion.minor >= 3;
	}

	void OpenGLRenderer::SetWindowHints()
	{
		window::Window& window = Application::GetInstance()->GetWindow();
		window.SetWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		window.SetWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		window.SetWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		window.SetWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		window.SetWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE);
	}

	smart_pointers::shared_ptr<StaticMesh> OpenGLRenderer::OnCreateStaticMesh()
	{
		return smart_pointers::make_shared<OpenGLStaticMesh>();
	}

	smart_pointers::shared_ptr<Material> OpenGLRenderer::OnCreateMaterial()
	{
		return smart_pointers::make_shared<OpenGLMaterial>();
	}

	smart_pointers::shared_ptr<ReservedUniformBuffers> OpenGLRenderer::OnCreateReversedUniformBuffers()
	{
		return smart_pointers::make_shared<OpenGLReservedUniformBuffers>();
	}

	smart_pointers::shared_ptr<Uniform> OpenGLRenderer::OnCreateUniform(EUniformType type)
	{
		switch (type)
		{
		case EUniformType::Float: return smart_pointers::make_shared<OpenGLUniformFloat>();
		case EUniformType::FVec2: return smart_pointers::make_shared<OpenGLUniformFVec2>();
		case EUniformType::FVec3: return smart_pointers::make_shared<OpenGLUniformFVec3>();
		case EUniformType::FVec4: return smart_pointers::make_shared<OpenGLUniformFVec4>();
		case EUniformType::Int: return smart_pointers::make_shared<OpenGLUniformInt>();
		case EUniformType::IVec2: return smart_pointers::make_shared<OpenGLUniformIVec2>();
		case EUniformType::IVec3: return smart_pointers::make_shared<OpenGLUniformIVec3>();
		case EUniformType::IVec4: return smart_pointers::make_shared<OpenGLUniformIVec4>();
		case EUniformType::UInt: return smart_pointers::make_shared<OpenGLUniformUInt>();
		case EUniformType::UVec2: return smart_pointers::make_shared<OpenGLUniformUVec2>();
		case EUniformType::UVec3: return smart_pointers::make_shared<OpenGLUniformUVec3>();
		case EUniformType::UVec4: return smart_pointers::make_shared<OpenGLUniformUVec4>();
		case EUniformType::FMat2: return smart_pointers::make_shared<OpenGLUniformFMat2>();
		case EUniformType::FMat3: return smart_pointers::make_shared<OpenGLUniformFMat3>();
		case EUniformType::FMat4: return smart_pointers::make_shared<OpenGLUniformFMat4>();
		case EUniformType::Texture2D: return smart_pointers::make_shared<OpenGLUniformTexture2D>();
		case EUniformType::Texture2DArray: return smart_pointers::make_shared<OpenGLUniformTexture2DArray>();
		case EUniformType::Texture3D: return smart_pointers::make_shared<OpenGLUniformTexture3D>();
		case EUniformType::TextureCubeMap: return smart_pointers::make_shared<OpenGLUniformTextureCubeMap>();
		default: return nullptr;
		}
	}

	smart_pointers::shared_ptr<UniformBuffer> OpenGLRenderer::OnCreateUniformBuffer()
	{
		return smart_pointers::make_shared<OpenGLUniformBuffer>();
	}

	smart_pointers::shared_ptr<ShaderProgram> OpenGLRenderer::OnCreateShaderProgram()
	{
		return smart_pointers::make_shared<OpenGLShaderProgram>();
	}

	smart_pointers::shared_ptr<DebugRenderer> OpenGLRenderer::OnCreateDebugRenderer()
	{
		return smart_pointers::make_shared<OpenGLDebugRenderer>();
	}

	smart_pointers::shared_ptr<Texture2D> OpenGLRenderer::OnCreateTexture2D()
	{
		return smart_pointers::make_shared<OpenGLTexture2D>();
	}

	smart_pointers::shared_ptr<Texture2DArray> OpenGLRenderer::OnCreateTexture2DArray()
	{
		return smart_pointers::make_shared<OpenGLTexture2DArray>();
	}

	smart_pointers::shared_ptr<Texture3D> OpenGLRenderer::OnCreateTexture3D()
	{
		return smart_pointers::make_shared<OpenGLTexture3D>();
	}

	smart_pointers::shared_ptr<TextureCubeMap> OpenGLRenderer::OnCreateTextureCubeMap()
	{
		return smart_pointers::make_shared<OpenGLTextureCubeMap>();
	}

	void OpenGLRenderer::OnInit()
	{
		window::Window& window = Application::GetInstance()->GetWindow();
		glfwMakeContextCurrent(window.GetNativeHandle());

		if (GLVersion.major == 0 && GLVersion.minor == 0 && !gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
			throw std::runtime_error("Could not load OpenGL!");

		Application::GetInstance()->GetWindow().SetVSync(false);
	}

	void OpenGLRenderer::OnDeInit()
	{
	}

	void OpenGLRenderer::OnBeginFrame()
	{
		const window::WindowData& windowData = Application::GetInstance()->GetWindow().GetWindowData();
		glViewport(0, 0, windowData.FramebufferWidth, windowData.FramebufferHeight);
	}

	void OpenGLRenderer::OnEndFrame()
	{
	}

	void OpenGLRenderer::OnRender(const smart_pointers::shared_ptr<scene::Camera>& camera)
	{
		if (!camera)
			return;

		//----
		// TODO(MarcasRealAccount): This should only be called for the main camera that's rendering to the window.
		const window::WindowData& windowData = Application::GetInstance()->GetWindow().GetWindowData();
		camera->m_Aspect                     = static_cast<float>(windowData.FramebufferWidth) / static_cast<float>(windowData.FramebufferHeight);
		glClearColor(camera->m_ClearColor.r, camera->m_ClearColor.g, camera->m_ClearColor.b, camera->m_ClearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		//----

		scene::Scene* scene = camera->GetScene();

		smart_pointers::shared_ptr<OpenGLReservedUniformBuffers> reservedUniformBuffers = smart_pointers::reinterpret_pointer_cast<OpenGLReservedUniformBuffers>(m_ReservedUniformBuffers);

		smart_pointers::shared_ptr<UniformFMat4> projectionViewMatrixUniform = reservedUniformBuffers->GetUniform<UniformFMat4>("Camera", "projectionViewMatrix");
		if (projectionViewMatrixUniform && projectionViewMatrixUniform->GetType() == UniformFMat4::GetTypeS())
			projectionViewMatrixUniform->SetValue(camera->GetProjectionViewMatrix());

		smart_pointers::shared_ptr<UniformFMat4> projectionMatrixUniform = reservedUniformBuffers->GetUniform<UniformFMat4>("Camera", "projectionMatrix");
		if (projectionMatrixUniform && projectionMatrixUniform->GetType() == UniformFMat4::GetTypeS())
			projectionMatrixUniform->SetValue(camera->GetProjectionMatrix());

		smart_pointers::shared_ptr<UniformFMat4> viewMatrixUniform = reservedUniformBuffers->GetUniform<UniformFMat4>("Camera", "viewMatrix");
		if (viewMatrixUniform && viewMatrixUniform->GetType() == UniformFMat4::GetTypeS())
			viewMatrixUniform->SetValue(camera->GetTransformationMatrix(true));

		reservedUniformBuffers->Bind();

		for (auto& weakEntity : scene->GetEntities())
		{
			if (!weakEntity.expired())
			{
				smart_pointers::shared_ptr<scene::Entity> entity = weakEntity.lock();
				if (entity->IsRenderable())
					RenderEntity(smart_pointers::reinterpret_pointer_cast<scene::RenderableEntity>(entity));
			}
		}

		smart_pointers::shared_ptr<OpenGLDebugRenderer> debugRenderer = smart_pointers::reinterpret_pointer_cast<OpenGLDebugRenderer>(m_DebugRenderer);
		debugRenderer->Render();

		glfwSwapBuffers(Application::GetInstance()->GetWindow().GetNativeHandle());
	}

	void OpenGLRenderer::RenderEntity(const smart_pointers::shared_ptr<scene::RenderableEntity>& entity)
	{
		smart_pointers::shared_ptr<OpenGLMaterial> material = smart_pointers::reinterpret_pointer_cast<OpenGLMaterial>(entity->GetMaterial());
		OpenGLMesh*                                mesh     = reinterpret_cast<OpenGLMesh*>(entity->GetMesh()->GetNext());

		smart_pointers::shared_ptr<UniformFMat4> transformationMatrixUniform = material->GetUniform<UniformFMat4>("Object", "transformationMatrix");
		if (transformationMatrixUniform && transformationMatrixUniform->GetType() == UniformFMat4::GetTypeS())
			transformationMatrixUniform->SetValue(entity->GetTransformationMatrix());

		material->Bind();
		mesh->Render();
		material->Unbind();
	}
} // namespace gp1::renderer::opengl

#endif