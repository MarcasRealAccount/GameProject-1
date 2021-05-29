//
//	Created by MarcasRealAccount on 13. May. 2021
//

#pragma once

#include "Engine/Renderer/DebugRenderer.h"
#include "Engine/Renderer/Material/Material.h"
#include "Engine/Renderer/Material/ReservedUniformBuffers.h"
#include "Engine/Renderer/Material/Uniform.h"
#include "Engine/Renderer/Material/UniformBuffer.h"
#include "Engine/Renderer/Mesh/StaticMesh.h"
#include "Engine/Renderer/Shader/ShaderProgram.h"
#include "Engine/Renderer/Texture/Texture2D.h"
#include "Engine/Renderer/Texture/Texture2DArray.h"
#include "Engine/Renderer/Texture/Texture3D.h"
#include "Engine/Renderer/Texture/TextureCubeMap.h"
#include "Engine/Scene/Camera.h"

#include <memory>
#include <vector>

namespace gp1::renderer
{
	class Renderer
	{
	public:
		virtual ~Renderer();

		StaticMesh*     CreateStaticMesh();
		Material*       CreateMaterial();
		Uniform*        CreateUniform(EUniformType type);
		UniformBuffer*  CreateUniformBuffer();
		ShaderProgram*  CreateShaderProgram();
		Texture2D*      CreateTexture2D();
		Texture2DArray* CreateTexture2DArray();
		Texture3D*      CreateTexture3D();
		TextureCubeMap* CreateTextureCubeMap();

		void RemoveRendererData(RendererData* data);

		virtual void SetWindowHints() = 0;

		virtual bool IsCompatible() const = 0;

		void Init();
		void DeInit();

		void BeginFrame();
		void EndFrame();
		void Render(scene::Camera* camera);

		inline ReservedUniformBuffers* GetReservedUniformBuffers() const
		{
			return m_ReservedUniformBuffers;
		}

		inline DebugRenderer* GetDebugRenderer() const
		{
			return m_DebugRenderer;
		}

	protected:
		Renderer() = default;

	protected:
		virtual StaticMesh*             OnCreateStaticMesh()               = 0;
		virtual Material*               OnCreateMaterial()                 = 0;
		virtual ReservedUniformBuffers* OnCreateReversedUniformBuffers()   = 0;
		virtual Uniform*                OnCreateUniform(EUniformType type) = 0;
		virtual UniformBuffer*          OnCreateUniformBuffer()            = 0;
		virtual ShaderProgram*          OnCreateShaderProgram()            = 0;
		virtual DebugRenderer*          OnCreateDebugRenderer()            = 0;
		virtual Texture2D*              OnCreateTexture2D()                = 0;
		virtual Texture2DArray*         OnCreateTexture2DArray()           = 0;
		virtual Texture3D*              OnCreateTexture3D()                = 0;
		virtual TextureCubeMap*         OnCreateTextureCubeMap()           = 0;

		virtual void OnInit() {}
		virtual void OnDeInit() {}

		virtual void OnBeginFrame() {}
		virtual void OnEndFrame() {}
		virtual void OnRender(scene::Camera* camera) = 0;

	protected:
		ReservedUniformBuffers* m_ReservedUniformBuffers;
		DebugRenderer*          m_DebugRenderer;

		std::vector<StaticMesh*>     m_StaticMeshes;
		std::vector<Material*>       m_Materials;
		std::vector<Uniform*>        m_Uniforms;
		std::vector<UniformBuffer*>  m_UniformBuffers;
		std::vector<ShaderProgram*>  m_ShaderPrograms;
		std::vector<Texture2D*>      m_Texture2Ds;
		std::vector<Texture2DArray*> m_Texture2DArrays;
		std::vector<Texture3D*>      m_Texture3Ds;
		std::vector<TextureCubeMap*> m_TextureCubeMaps;

		std::vector<RendererData*> m_UpdatableRendererDatas;
	};
} // namespace gp1::renderer