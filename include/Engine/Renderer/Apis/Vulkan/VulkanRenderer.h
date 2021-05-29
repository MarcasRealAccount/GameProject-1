#pragma once

#include "Engine/Utility/Core.h"

#ifdef RENDERER_VULKAN

#include "Engine/Renderer/Renderer.h"
#include "Engine/Utility/SmartPointers/SmartPointers.h"

namespace gp1::renderer::vulkan
{
	class VulkanRenderer : public Renderer
	{
	public:
		virtual bool IsCompatible() const override;

		virtual void SetWindowHints() override;

	protected:
		virtual smart_pointers::shared_ptr<StaticMesh>             OnCreateStaticMesh() override;
		virtual smart_pointers::shared_ptr<Material>               OnCreateMaterial() override;
		virtual smart_pointers::shared_ptr<ReservedUniformBuffers> OnCreateReversedUniformBuffers() override;
		virtual smart_pointers::shared_ptr<Uniform>                OnCreateUniform(EUniformType type) override;
		virtual smart_pointers::shared_ptr<UniformBuffer>          OnCreateUniformBuffer() override;
		virtual smart_pointers::shared_ptr<ShaderProgram>          OnCreateShaderProgram() override;
		virtual smart_pointers::shared_ptr<DebugRenderer>          OnCreateDebugRenderer() override;
		virtual smart_pointers::shared_ptr<Texture2D>              OnCreateTexture2D() override;
		virtual smart_pointers::shared_ptr<Texture2DArray>         OnCreateTexture2DArray() override;
		virtual smart_pointers::shared_ptr<Texture3D>              OnCreateTexture3D() override;
		virtual smart_pointers::shared_ptr<TextureCubeMap>         OnCreateTextureCubeMap() override;

		virtual void OnInit() override;
		virtual void OnDeInit() override;

		virtual void OnBeginFrame() override;
		virtual void OnEndFrame() override;
		virtual void OnRender(const smart_pointers::shared_ptr<scene::Camera>& camera) override;
	};
} // namespace gp1::renderer::vulkan

#endif