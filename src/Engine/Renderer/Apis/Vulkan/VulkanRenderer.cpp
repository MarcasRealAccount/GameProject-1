#include "Engine/Utility/Core.h"

#ifdef RENDERER_VULKAN

#include "Engine/Application.h"
#include "Engine/Renderer/Apis/Vulkan/Material/VulkanMaterial.h"
#include "Engine/Renderer/Apis/Vulkan/Material/VulkanReservedUniformBuffers.h"
#include "Engine/Renderer/Apis/Vulkan/Material/VulkanUniform.h"
#include "Engine/Renderer/Apis/Vulkan/Material/VulkanUniformBuffer.h"
#include "Engine/Renderer/Apis/Vulkan/Mesh/VulkanStaticMesh.h"
#include "Engine/Renderer/Apis/Vulkan/Shader/VulkanShaderProgram.h"
#include "Engine/Renderer/Apis/Vulkan/Texture/VulkanTexture2D.h"
#include "Engine/Renderer/Apis/Vulkan/Texture/VulkanTexture2DArray.h"
#include "Engine/Renderer/Apis/Vulkan/Texture/VulkanTexture3D.h"
#include "Engine/Renderer/Apis/Vulkan/Texture/VulkanTextureCubeMap.h"
#include "Engine/Renderer/Apis/Vulkan/VulkanDebugRenderer.h"
#include "Engine/Renderer/Apis/Vulkan/VulkanRenderer.h"
#include "Engine/Scene/Camera.h"
#include "Engine/Window/Window.h"

namespace gp1::renderer::vulkan
{
	bool VulkanRenderer::IsCompatible() const
	{
		return false;
	}

	void VulkanRenderer::SetWindowHints()
	{
		window::Window& window = Application::GetInstance()->GetWindow();
		window.SetWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}

	StaticMesh* VulkanRenderer::OnCreateStaticMesh()
	{
		return new VulkanStaticMesh();
	}

	Material* VulkanRenderer::OnCreateMaterial()
	{
		return new VulkanMaterial();
	}

	ReservedUniformBuffers* VulkanRenderer::OnCreateReversedUniformBuffers()
	{
		return new VulkanReservedUniformBuffers();
	}

	Uniform* VulkanRenderer::OnCreateUniform(EUniformType type)
	{
		switch (type)
		{
		case EUniformType::Float: return new VulkanUniformFloat();
		case EUniformType::FVec2: return new VulkanUniformFVec2();
		case EUniformType::FVec3: return new VulkanUniformFVec3();
		case EUniformType::FVec4: return new VulkanUniformFVec4();
		case EUniformType::Int: return new VulkanUniformInt();
		case EUniformType::IVec2: return new VulkanUniformIVec2();
		case EUniformType::IVec3: return new VulkanUniformIVec3();
		case EUniformType::IVec4: return new VulkanUniformIVec4();
		case EUniformType::UInt: return new VulkanUniformUInt();
		case EUniformType::UVec2: return new VulkanUniformUVec2();
		case EUniformType::UVec3: return new VulkanUniformUVec3();
		case EUniformType::UVec4: return new VulkanUniformUVec4();
		case EUniformType::FMat2: return new VulkanUniformFMat2();
		case EUniformType::FMat3: return new VulkanUniformFMat3();
		case EUniformType::FMat4: return new VulkanUniformFMat4();
		case EUniformType::Texture2D: return new VulkanUniformTexture2D();
		case EUniformType::Texture2DArray: return new VulkanUniformTexture2DArray();
		case EUniformType::Texture3D: return new VulkanUniformTexture3D();
		case EUniformType::TextureCubeMap: return new VulkanUniformTextureCubeMap();
		default: return nullptr;
		}
	}

	UniformBuffer* VulkanRenderer::OnCreateUniformBuffer()
	{
		return new VulkanUniformBuffer();
	}

	ShaderProgram* VulkanRenderer::OnCreateShaderProgram()
	{
		return new VulkanShaderProgram();
	}

	DebugRenderer* VulkanRenderer::OnCreateDebugRenderer()
	{
		return new VulkanDebugRenderer();
	}

	Texture2D* VulkanRenderer::OnCreateTexture2D()
	{
		return new VulkanTexture2D();
	}

	Texture2DArray* VulkanRenderer::OnCreateTexture2DArray()
	{
		return new VulkanTexture2DArray();
	}

	Texture3D* VulkanRenderer::OnCreateTexture3D()
	{
		return new VulkanTexture3D();
	}

	TextureCubeMap* VulkanRenderer::OnCreateTextureCubeMap()
	{
		return new VulkanTextureCubeMap();
	}

	void VulkanRenderer::OnInit()
	{
	}

	void VulkanRenderer::OnDeInit()
	{
	}

	void VulkanRenderer::OnBeginFrame()
	{
	}

	void VulkanRenderer::OnEndFrame()
	{
	}

	void VulkanRenderer::OnRender(scene::Camera* camera)
	{
		[[maybe_unused]] scene::Scene* scene = camera->GetScene();
	}
} // namespace gp1::renderer::vulkan

#endif