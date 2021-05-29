//
//	Created by MarcasRealAccount on 14. May. 2021
//

#pragma once

#include "Engine/Renderer/Material/UniformBuffer.h"
#include "Engine/Renderer/RendererData.h"
#include "Engine/Renderer/Shader/ShaderProgram.h"
#include "Engine/Utility/SmartPointers/SmartPointers.h"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace gp1::renderer
{
	enum class ETriangleFace : uint32_t
	{
		Front,
		Back,
		FrontAndBack
	};

	enum class EBlendFunc : uint32_t
	{
		Zero,
		One,
		SrcColor,
		OneMinusSrcColor,
		DstColor,
		OneMinusDstColor,
		SrcAlpha,
		OneMinusSrcAlpha,
		DstAlpha,
		OneMinusDstAlpha,
		ConstantColor,
		OneMinusConstantColor,
		ConstantAlpha,
		OneMinusConstantAlpha,
		SrcAlphaSaturate,
		Src1Color,
		OneMinusSrc1Color,
		Src1Alpha,
		OneMinusSrc1Alpha
	};

	enum class EPolygonMode : uint32_t
	{
		Point,
		Line,
		Fill
	};

	struct CullMode
	{
	public:
		bool          m_Enabled = true;
		ETriangleFace m_Face    = ETriangleFace::Back;
	};

	struct BlendFunc
	{
	public:
		bool       m_Enabled = true;
		EBlendFunc m_SrcFunc = EBlendFunc::SrcAlpha;
		EBlendFunc m_DstFunc = EBlendFunc::OneMinusSrcAlpha;
	};

	struct PolygonMode
	{
	public:
		bool          m_Enabled = true;
		ETriangleFace m_Face    = ETriangleFace::FrontAndBack;
		EPolygonMode  m_Mode    = EPolygonMode::Fill;
	};

	struct Material : public RendererData
	{
	public:
		friend ShaderProgram;

		struct UniformBufferEntry
		{
		public:
			std::string                               m_Name;
			smart_pointers::shared_ptr<UniformBuffer> m_UniformBuffer;
		};

	public:
		static smart_pointers::shared_ptr<Material> Create();

	public:
		virtual ~Material() = default;

		void SetShaderProgram(const smart_pointers::shared_ptr<ShaderProgram>& shaderProgram);

		inline smart_pointers::shared_ptr<ShaderProgram> GetShaderProgram() const
		{
			return m_ShaderProgram;
		}

		smart_pointers::shared_ptr<UniformBuffer> GetUniformBuffer(std::string_view name) const;
		smart_pointers::shared_ptr<Uniform>       GetUniform(std::string_view bufferName, std::string_view uniformName) const;

		template <typename T, std::enable_if_t<std::is_base_of_v<Uniform, T>, bool> = true>
		inline smart_pointers::shared_ptr<T> GetUniform(std::string_view bufferName, const std::string_view uniformName) const
		{
			return smart_pointers::reinterpret_pointer_cast<T>(GetUniform(bufferName, uniformName));
		}

	protected:
		Material() = default;

		void UpdateUniformBuffers(std::vector<std::pair<std::string, std::vector<std::pair<std::string, EUniformType>>>> uniformBuffers);

	public:
		bool        m_DepthTest = false;
		CullMode    m_CullMode;
		BlendFunc   m_BlendFunc;
		PolygonMode m_PolygonMode;

	protected:
		std::vector<UniformBufferEntry> m_UniformBuffers;

	private:
		smart_pointers::shared_ptr<ShaderProgram> m_ShaderProgram;
	};
} // namespace gp1::renderer