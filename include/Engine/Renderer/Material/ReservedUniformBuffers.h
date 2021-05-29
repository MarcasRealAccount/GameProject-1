//
//	Created by MarcasRealAccount on 16. May. 2021
//

#pragma once

#include "Engine/Renderer/Material/UniformBuffer.h"

namespace gp1::renderer
{
	struct ReservedUniformBuffers : public RendererData
	{
	public:
		static constexpr const uint32_t MaxUniformBufferCount = 8;

		struct UniformBufferEntry
		{
		public:
			UniformBufferEntry() = default;
			UniformBufferEntry(const std::string& name, UniformBuffer* uniformBuffer);
			UniformBufferEntry(const UniformBufferEntry&) = default;
			UniformBufferEntry(UniformBufferEntry&& move) noexcept;
			UniformBufferEntry& operator=(const UniformBufferEntry&) = default;

			UniformBufferEntry& operator=(UniformBufferEntry&& move) noexcept;
			~UniformBufferEntry();

		public:
			std::string    m_Name;
			UniformBuffer* m_UniformBuffer = nullptr;
		};

	public:
		virtual ~ReservedUniformBuffers() = default;

		UniformBuffer* GetUniformBuffer(const std::string& name) const;
		Uniform*       GetUniform(const std::string& bufferName, const std::string& uniformName) const;

		template <typename T, std::enable_if_t<std::is_base_of_v<Uniform, T>, bool> = true>
		T* GetUniform(const std::string& bufferName, const std::string& uniformName) const
		{
			return reinterpret_cast<T*>(GetUniform(bufferName, uniformName));
		}

	protected:
		ReservedUniformBuffers();

	protected:
		UniformBufferEntry m_UniformBuffers[MaxUniformBufferCount];
	};
} // namespace gp1::renderer