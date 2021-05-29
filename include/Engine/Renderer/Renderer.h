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
#include "Engine/Utility/SmartPointers/SmartPointers.h"

#include <vector>

namespace gp1::renderer
{
	class Renderer
	{
	public:
		virtual ~Renderer() = default;

		smart_pointers::shared_ptr<StaticMesh>     CreateStaticMesh();
		smart_pointers::shared_ptr<Material>       CreateMaterial();
		smart_pointers::shared_ptr<Uniform>        CreateUniform(EUniformType type);
		smart_pointers::shared_ptr<UniformBuffer>  CreateUniformBuffer();
		smart_pointers::shared_ptr<ShaderProgram>  CreateShaderProgram();
		smart_pointers::shared_ptr<Texture2D>      CreateTexture2D();
		smart_pointers::shared_ptr<Texture2DArray> CreateTexture2DArray();
		smart_pointers::shared_ptr<Texture3D>      CreateTexture3D();
		smart_pointers::shared_ptr<TextureCubeMap> CreateTextureCubeMap();

		virtual void SetWindowHints() = 0;

		virtual bool IsCompatible() const = 0;

		void Init();
		void DeInit();

		void BeginFrame();
		void EndFrame();
		void Render(const smart_pointers::shared_ptr<scene::Camera>& camera);

		inline smart_pointers::shared_ptr<ReservedUniformBuffers> GetReservedUniformBuffers() const
		{
			return m_ReservedUniformBuffers;
		}

		inline smart_pointers::shared_ptr<DebugRenderer> GetDebugRenderer() const
		{
			return m_DebugRenderer;
		}

	protected:
		Renderer() = default;

	protected:
		virtual smart_pointers::shared_ptr<StaticMesh>             OnCreateStaticMesh()               = 0;
		virtual smart_pointers::shared_ptr<Material>               OnCreateMaterial()                 = 0;
		virtual smart_pointers::shared_ptr<ReservedUniformBuffers> OnCreateReversedUniformBuffers()   = 0;
		virtual smart_pointers::shared_ptr<Uniform>                OnCreateUniform(EUniformType type) = 0;
		virtual smart_pointers::shared_ptr<UniformBuffer>          OnCreateUniformBuffer()            = 0;
		virtual smart_pointers::shared_ptr<ShaderProgram>          OnCreateShaderProgram()            = 0;
		virtual smart_pointers::shared_ptr<DebugRenderer>          OnCreateDebugRenderer()            = 0;
		virtual smart_pointers::shared_ptr<Texture2D>              OnCreateTexture2D()                = 0;
		virtual smart_pointers::shared_ptr<Texture2DArray>         OnCreateTexture2DArray()           = 0;
		virtual smart_pointers::shared_ptr<Texture3D>              OnCreateTexture3D()                = 0;
		virtual smart_pointers::shared_ptr<TextureCubeMap>         OnCreateTextureCubeMap()           = 0;

		virtual void OnInit() {}
		virtual void OnDeInit() {}

		virtual void OnBeginFrame() {}
		virtual void OnEndFrame() {}
		virtual void OnRender(const smart_pointers::shared_ptr<scene::Camera>& camera) = 0;

	protected:
		smart_pointers::shared_ptr<ReservedUniformBuffers> m_ReservedUniformBuffers;
		smart_pointers::shared_ptr<DebugRenderer>          m_DebugRenderer;

		std::vector<smart_pointers::weak_ptr<StaticMesh>>     m_StaticMeshes;
		std::vector<smart_pointers::weak_ptr<Material>>       m_Materials;
		std::vector<smart_pointers::weak_ptr<Uniform>>        m_Uniforms;
		std::vector<smart_pointers::weak_ptr<UniformBuffer>>  m_UniformBuffers;
		std::vector<smart_pointers::weak_ptr<ShaderProgram>>  m_ShaderPrograms;
		std::vector<smart_pointers::weak_ptr<Texture2D>>      m_Texture2Ds;
		std::vector<smart_pointers::weak_ptr<Texture2DArray>> m_Texture2DArrays;
		std::vector<smart_pointers::weak_ptr<Texture3D>>      m_Texture3Ds;
		std::vector<smart_pointers::weak_ptr<TextureCubeMap>> m_TextureCubeMaps;

		std::vector<smart_pointers::weak_ptr<RendererData>> m_UpdatableRendererDatas;
	};
} // namespace gp1::renderer