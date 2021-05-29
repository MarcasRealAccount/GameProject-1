//
//	Created by MarcasRealAccount on 13. May. 2021
//

#include "Engine/Renderer/Renderer.h"

#include <algorithm>

namespace gp1::renderer
{
	Renderer::~Renderer()
	{
		if (m_ReservedUniformBuffers)
			delete m_ReservedUniformBuffers;
		if (m_DebugRenderer)
			delete m_DebugRenderer;
	}

	StaticMesh* Renderer::CreateStaticMesh()
	{
		StaticMesh* mesh = OnCreateStaticMesh();
		if (mesh)
		{
			m_StaticMeshes.push_back(mesh);
			static_cast<RendererData*>(mesh)->m_Renderer = this;

			if (mesh->IsUpdatable())
				m_UpdatableRendererDatas.push_back(mesh);
		}
		return mesh;
	}

	Material* Renderer::CreateMaterial()
	{
		Material* material = OnCreateMaterial();
		if (material)
		{
			m_Materials.push_back(material);
			static_cast<RendererData*>(material)->m_Renderer = this;

			if (material->IsUpdatable())
				m_UpdatableRendererDatas.push_back(material);
		}
		return material;
	}

	Uniform* Renderer::CreateUniform(EUniformType type)
	{
		Uniform* uniform = OnCreateUniform(type);
		if (uniform)
		{
			m_Uniforms.push_back(uniform);
			static_cast<RendererData*>(uniform)->m_Renderer = this;

			if (uniform->IsUpdatable())
				m_UpdatableRendererDatas.push_back(uniform);
		}
		return uniform;
	}

	UniformBuffer* Renderer::CreateUniformBuffer()
	{
		UniformBuffer* uniformBuffer = OnCreateUniformBuffer();
		if (uniformBuffer)
		{
			m_UniformBuffers.push_back(uniformBuffer);
			static_cast<RendererData*>(uniformBuffer)->m_Renderer = this;

			if (uniformBuffer->IsUpdatable())
				m_UpdatableRendererDatas.push_back(uniformBuffer);
		}
		return uniformBuffer;
	}

	ShaderProgram* Renderer::CreateShaderProgram()
	{
		ShaderProgram* shaderProgram = OnCreateShaderProgram();
		if (shaderProgram)
		{
			m_ShaderPrograms.push_back(shaderProgram);
			static_cast<RendererData*>(shaderProgram)->m_Renderer = this;

			if (shaderProgram->IsUpdatable())
				m_UpdatableRendererDatas.push_back(shaderProgram);
		}
		return shaderProgram;
	}

	Texture2D* Renderer::CreateTexture2D()
	{
		Texture2D* texture2D = OnCreateTexture2D();
		if (texture2D)
		{
			m_Texture2Ds.push_back(texture2D);
			static_cast<RendererData*>(texture2D)->m_Renderer = this;

			if (texture2D->IsUpdatable())
				m_UpdatableRendererDatas.push_back(texture2D);
		}
		return texture2D;
	}

	Texture2DArray* Renderer::CreateTexture2DArray()
	{
		Texture2DArray* texture2DArray = OnCreateTexture2DArray();
		if (texture2DArray)
		{
			m_Texture2DArrays.push_back(texture2DArray);
			static_cast<RendererData*>(texture2DArray)->m_Renderer = this;

			if (texture2DArray->IsUpdatable())
				m_UpdatableRendererDatas.push_back(texture2DArray);
		}
		return texture2DArray;
	}

	Texture3D* Renderer::CreateTexture3D()
	{
		Texture3D* texture3D = OnCreateTexture3D();
		if (texture3D)
		{
			m_Texture3Ds.push_back(texture3D);
			static_cast<RendererData*>(texture3D)->m_Renderer = this;

			if (texture3D->IsUpdatable())
				m_UpdatableRendererDatas.push_back(texture3D);
		}
		return texture3D;
	}

	TextureCubeMap* Renderer::CreateTextureCubeMap()
	{
		TextureCubeMap* textureCubeMap = OnCreateTextureCubeMap();
		if (textureCubeMap)
		{
			m_TextureCubeMaps.push_back(textureCubeMap);
			static_cast<RendererData*>(textureCubeMap)->m_Renderer = this;

			if (textureCubeMap->IsUpdatable())
				m_UpdatableRendererDatas.push_back(textureCubeMap);
		}
		return textureCubeMap;
	}

	template <typename T>
	static void erasePtrFromVector(std::vector<T*>& vec, const void* element)
	{
		auto itr = std::find(vec.begin(), vec.end(), reinterpret_cast<const T*>(element));
		if (itr != vec.end())
			vec.erase(itr);
	}

	void Renderer::RemoveRendererData(RendererData* data)
	{
		erasePtrFromVector(m_StaticMeshes, data);
		erasePtrFromVector(m_Materials, data);
		erasePtrFromVector(m_Uniforms, data);
		erasePtrFromVector(m_UniformBuffers, data);
		erasePtrFromVector(m_ShaderPrograms, data);
		erasePtrFromVector(m_Texture2Ds, data);
		erasePtrFromVector(m_Texture2DArrays, data);
		erasePtrFromVector(m_Texture3Ds, data);
		erasePtrFromVector(m_TextureCubeMaps, data);
		erasePtrFromVector(m_UpdatableRendererDatas, data);
		data->m_Renderer = nullptr;
	}

	void Renderer::Init()
	{
		OnInit();

		m_DebugRenderer = OnCreateDebugRenderer();
		DebugRenderer::SetDebugRenderer(m_DebugRenderer);

		m_ReservedUniformBuffers = OnCreateReversedUniformBuffers();
	}

	void Renderer::DeInit()
	{
		DebugRenderer::SetDebugRenderer(nullptr);

		OnDeInit();
	}

	void Renderer::BeginFrame()
	{
		for (auto data : m_UpdatableRendererDatas)
			data->Update();

		OnBeginFrame();
	}

	void Renderer::EndFrame()
	{
		OnEndFrame();
	}

	void Renderer::Render(scene::Camera* camera)
	{
		OnRender(camera);
	}
} // namespace gp1::renderer