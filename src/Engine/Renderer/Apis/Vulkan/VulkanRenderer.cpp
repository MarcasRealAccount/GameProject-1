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
#include "Engine/Utility/SmartPointers/SmartPointers.h"
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

	smart_pointers::shared_ptr<StaticMesh> VulkanRenderer::OnCreateStaticMesh()
	{
		return smart_pointers::make_shared<VulkanStaticMesh>();
	}

	smart_pointers::shared_ptr<Material> VulkanRenderer::OnCreateMaterial()
	{
		return smart_pointers::make_shared<VulkanMaterial>();
	}

	smart_pointers::shared_ptr<ReservedUniformBuffers> VulkanRenderer::OnCreateReversedUniformBuffers()
	{
		return smart_pointers::make_shared<VulkanReservedUniformBuffers>();
	}

	smart_pointers::shared_ptr<Uniform> VulkanRenderer::OnCreateUniform(EUniformType type)
	{
		switch (type)
		{
		case EUniformType::Float: return smart_pointers::make_shared<VulkanUniformFloat>();
		case EUniformType::FVec2: return smart_pointers::make_shared<VulkanUniformFVec2>();
		case EUniformType::FVec3: return smart_pointers::make_shared<VulkanUniformFVec3>();
		case EUniformType::FVec4: return smart_pointers::make_shared<VulkanUniformFVec4>();
		case EUniformType::Int: return smart_pointers::make_shared<VulkanUniformInt>();
		case EUniformType::IVec2: return smart_pointers::make_shared<VulkanUniformIVec2>();
		case EUniformType::IVec3: return smart_pointers::make_shared<VulkanUniformIVec3>();
		case EUniformType::IVec4: return smart_pointers::make_shared<VulkanUniformIVec4>();
		case EUniformType::UInt: return smart_pointers::make_shared<VulkanUniformUInt>();
		case EUniformType::UVec2: return smart_pointers::make_shared<VulkanUniformUVec2>();
		case EUniformType::UVec3: return smart_pointers::make_shared<VulkanUniformUVec3>();
		case EUniformType::UVec4: return smart_pointers::make_shared<VulkanUniformUVec4>();
		case EUniformType::FMat2: return smart_pointers::make_shared<VulkanUniformFMat2>();
		case EUniformType::FMat3: return smart_pointers::make_shared<VulkanUniformFMat3>();
		case EUniformType::FMat4: return smart_pointers::make_shared<VulkanUniformFMat4>();
		case EUniformType::Texture2D: return smart_pointers::make_shared<VulkanUniformTexture2D>();
		case EUniformType::Texture2DArray: return smart_pointers::make_shared<VulkanUniformTexture2DArray>();
		case EUniformType::Texture3D: return smart_pointers::make_shared<VulkanUniformTexture3D>();
		case EUniformType::TextureCubeMap: return smart_pointers::make_shared<VulkanUniformTextureCubeMap>();
		default: return nullptr;
		}
	}

	smart_pointers::shared_ptr<UniformBuffer> VulkanRenderer::OnCreateUniformBuffer()
	{
		return smart_pointers::make_shared<VulkanUniformBuffer>();
	}

	smart_pointers::shared_ptr<ShaderProgram> VulkanRenderer::OnCreateShaderProgram()
	{
		return smart_pointers::make_shared<VulkanShaderProgram>();
	}

	smart_pointers::shared_ptr<DebugRenderer> VulkanRenderer::OnCreateDebugRenderer()
	{
		return smart_pointers::make_shared<VulkanDebugRenderer>();
	}

	smart_pointers::shared_ptr<Texture2D> VulkanRenderer::OnCreateTexture2D()
	{
		return smart_pointers::make_shared<VulkanTexture2D>();
	}

	smart_pointers::shared_ptr<Texture2DArray> VulkanRenderer::OnCreateTexture2DArray()
	{
		return smart_pointers::make_shared<VulkanTexture2DArray>();
	}

	smart_pointers::shared_ptr<Texture3D> VulkanRenderer::OnCreateTexture3D()
	{
		return smart_pointers::make_shared<VulkanTexture3D>();
	}

	smart_pointers::shared_ptr<TextureCubeMap> VulkanRenderer::OnCreateTextureCubeMap()
	{
		return smart_pointers::make_shared<VulkanTextureCubeMap>();
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

	void VulkanRenderer::OnRender(const smart_pointers::shared_ptr<scene::Camera>& camera)
	{
		[[maybe_unused]] scene::Scene* scene = camera->GetScene();
	}
} // namespace gp1::renderer::vulkan

#endif