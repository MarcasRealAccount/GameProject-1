//
//	Created by MarcasRealAccount on 19. May. 2021
//

#pragma once

#include "Engine/Renderer/Material/Uniform.h"
#include "Engine/Renderer/RendererData.h"

#include <memory>
#include <string>
#include <vector>

namespace gp1::renderer
{
	struct UniformBuffer : RendererData
	{
	public:
		struct UniformInfo
		{
		public:
			UniformInfo() = default;
			UniformInfo(const std::string& name, Uniform* uniform);
			UniformInfo(const UniformInfo& move) = default;
			UniformInfo(UniformInfo&& move) noexcept;
			UniformInfo& operator=(const UniformInfo& move) = default;

			UniformInfo& operator=(UniformInfo&& move) noexcept;
			~UniformInfo();

		public:
			std::string m_Name;
			Uniform*    m_Uniform = nullptr;
		};

		static UniformBuffer* Create();

	public:
		void UpdateUniforms(std::vector<std::pair<std::string, EUniformType>> uniformTypes);

		bool IsDirty() const;
		void ClearDirty();

		Uniform* GetUniform(std::string_view name) const;
		template <typename T, std::enable_if_t<std::is_base_of_v<Uniform, T>, bool> = true>
		T* GetUniform(std::string_view name) const
		{
			return reinterpret_cast<T*>(GetUniform(name));
		}

	protected:
		UniformBuffer() = default;

	protected:
		std::vector<UniformInfo> m_Uniforms;
	};
} // namespace gp1::renderer