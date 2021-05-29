//
//	Created by MarcasRealAccount on 13. May. 2021
//

#pragma once

#include "Engine/Utility/Core.h"

#ifdef RENDERER_OPENGL

#include "Engine/Renderer/Renderer.h"
#include "Engine/Scene/RenderableEntity.h"

#include <memory>

namespace gp1::renderer::opengl
{
	class OpenGLRenderer : public Renderer
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

	public:
		void RenderEntity(scene::RenderableEntity* entity);
	};
} // namespace gp1::renderer::opengl

#endif