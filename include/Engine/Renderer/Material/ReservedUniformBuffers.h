//
//	Created by MarcasRealAccount on 16. May. 2021
//

#pragma once

#include "Engine/Renderer/Material/UniformBuffer.h"
#include "Engine/Utility/SmartPointers/SmartPointers.h"

namespace gp1::renderer
{
	struct ReservedUniformBuffers : public RendererData
	{
	public:
		static constexpr const uint32_t MaxUniformBufferCount = 8;

		struct UniformBufferEntry
		{
		public:
			std::string                               m_Name;
			smart_pointers::shared_ptr<UniformBuffer> m_UniformBuffer;
		};

	public:
		virtual ~ReservedUniformBuffers() = default;

		smart_pointers::shared_ptr<UniformBuffer> GetUniformBuffer(const std::string& name) const;
		smart_pointers::shared_ptr<Uniform>       GetUniform(const std::string& bufferName, const std::string& uniformName) const;

		template <typename T, std::enable_if_t<std::is_base_of_v<Uniform, T>, bool> = true>
		smart_pointers::shared_ptr<T> GetUniform(const std::string& bufferName, const std::string& uniformName) const
		{
			return smart_pointers::reinterpret_pointer_cast<T>(GetUniform(bufferName, uniformName));
		}

	protected:
		ReservedUniformBuffers();

	protected:
		UniformBufferEntry m_UniformBuffers[MaxUniformBufferCount];
	};
} // namespace gp1::renderer