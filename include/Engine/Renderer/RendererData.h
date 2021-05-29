//
//	Created by MarcasRealAccount on 13. May. 2021
//

#pragma once

#include "Engine/Utility/SmartPointers/SmartPointers.h"

#include <type_traits>

namespace gp1::renderer
{
	struct RendererData
	{
	public:
		friend class Renderer;

	public:
		virtual ~RendererData() = default;

		inline void* GetNext() const
		{
			return m_Next;
		}

		template <typename T>
		inline T* GetNext() const
		{
			return reinterpret_cast<T*>(m_Next);
		}

		inline smart_pointers::shared_ptr<RendererData> GetThis() const
		{
			if (m_This.expired())
				return nullptr;
			else
				return m_This.lock();
		}

		template <typename T, std::enable_if_t<std::is_base_of_v<RendererData, T>, bool> = true>
		inline smart_pointers::shared_ptr<T> GetThis() const
		{
			if (m_This.expired())
				return nullptr;
			else
				return smart_pointers::reinterpret_pointer_cast<T>(m_This.lock());
		}

		virtual void Update() {}
		virtual bool IsUpdatable()
		{
			return false;
		}

	protected:
		RendererData() = default;

	protected:
		void* m_Next = nullptr;

		smart_pointers::weak_ptr<RendererData> m_This;
	};
} // namespace gp1::renderer