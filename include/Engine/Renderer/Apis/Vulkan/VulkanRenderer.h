#pragma once

#include "Engine/Utility/Core.h"

#ifdef RENDERER_VULKAN

#include "Engine/Renderer/Renderer.h"

#include <memory>

namespace gp1::renderer::vulkan
{
	class VulkanRenderer : public Renderer
	{
	public:
		virtual bool IsCompatible() const override;

		virtual void SetWindowHints() override;

	protected:
		virtual StaticMesh*             OnCreateStaticMesh() override;
		virtual Material*               OnCreateMaterial() override;
		virtual ReservedUniformBuffers* OnCreateReversedUniformBuffers() override;
		virtual Uniform*                OnCreateUniform(EUniformType type) override;
		virtual UniformBuffer*          OnCreateUniformBuffer() override;
		virtual ShaderProgram*          OnCreateShaderProgram() override;
		virtual DebugRenderer*          OnCreateDebugRenderer() override;
		virtual Texture2D*              OnCreateTexture2D() override;
		virtual Texture2DArray*         OnCreateTexture2DArray() override;
		virtual Texture3D*              OnCreateTexture3D() override;
		virtual TextureCubeMap*         OnCreateTextureCubeMap() override;

		virtual void OnInit() override;
		virtual void OnDeInit() override;

		virtual void OnBeginFrame() override;
		virtual void OnEndFrame() override;
		virtual void OnRender(scene::Camera* camera) override;
	};
} // namespace gp1::renderer::vulkan

#endif