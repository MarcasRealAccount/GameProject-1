#pragma once

#include "Engine/Utility/SmartPointers/Control.h"

#include <type_traits>

namespace smart_pointers
{
	template <class T>
	struct shared_ptr;

	template <class T>
	struct weak_ptr
	{
	public:
		using element_type    = std::remove_extent_t<T>;
		using shared_ptr_type = shared_ptr<T>;

		template <class U>
		friend struct shared_ptr;
		template <class U>
		friend struct weak_ptr;

	public:
		constexpr weak_ptr() noexcept = default;

		weak_ptr(const weak_ptr& copy) noexcept
		{
			__CopyConstruct(copy);
		}

		template <class U, std::enable_if_t<SharedPtrConvertible<U, T>::value, int> = 0>
		weak_ptr(const weak_ptr<U>& copy) noexcept
		{
			__CopyConstruct(copy);
		}

		template <class U, std::enable_if_t<SharedPtrConvertible<U, T>::value, int> = 0>
		weak_ptr(const shared_ptr<U>& copy) noexcept
		{
			__CopyConstruct(copy);
		}

		weak_ptr(weak_ptr&& move) noexcept
		{
			__MoveConstruct(std::move(move));
		}

		template <class U, std::enable_if_t<SharedPtrConvertible<U, T>::value, int> = 0>
		weak_ptr(weak_ptr<U>&& move) noexcept
		{
			__MoveConstruct(std::move(move));
		}

		~weak_ptr()
		{
			if (m_Control && --m_Control->m_Weak == 0)
				m_Control->DeleteThis();
		}

		weak_ptr& operator=(const weak_ptr& copy) noexcept
		{
			return __CopyAssign(copy);
		}

		template <class U, std::enable_if_t<SharedPtrConvertible<U, T>::value, int> = 0>
		weak_ptr& operator=(const weak_ptr<U>& copy) noexcept
		{
			return __CopyAssign(copy);
		}

		template <class U, std::enable_if_t<SharedPtrConvertible<U, T>::value, int> = 0>
		weak_ptr& operator=(const shared_ptr<U>& copy) noexcept
		{
			return __CopyAssign(copy);
		}

		weak_ptr& operator=(weak_ptr&& move) noexcept
		{
			return __MoveAssign(std::move(move));
		}

		template <class U, std::enable_if_t<SharedPtrConvertible<U, T>::value, int> = 0>
		weak_ptr& operator=(weak_ptr<U>&& move) noexcept
		{
			return __MoveAssign(std::move(move));
		}

		void reset() noexcept
		{
			if (m_Control && --m_Control->m_Weak == 0)
				m_Control->DeleteThis();
			m_Ptr     = nullptr;
			m_Control = nullptr;
		}

		void swap(weak_ptr& other) noexcept
		{
			std::swap(m_Ptr, other.m_Ptr);
			std::swap(m_Control, other.m_Control);
		}

		long use_count() const noexcept
		{
			if (m_Control) return m_Control->StrongCount();
			return 0;
		}

		bool expired() const noexcept
		{
			return use_count() == 0;
		}

		shared_ptr_type lock() const noexcept
		{
			return shared_ptr_type(*this);
		}

		template <class U>
		bool owner_before(const weak_ptr<U>& other) const noexcept
		{
			return m_Control != other.m_Control;
		}

	private:
		template <class U>
		void __CopyConstruct(const weak_ptr<U>& copy) noexcept
		{
			m_Ptr     = copy.m_Ptr;
			m_Control = copy.m_Control;
			if (m_Control) m_Control->m_Weak++;
		}

		template <class U>
		void __CopyConstruct(const shared_ptr<U>& copy) noexcept
		{
			m_Ptr     = copy.m_Ptr;
			m_Control = copy.m_Control;
			if (m_Control) m_Control->m_Weak++;
		}

		template <class U>
		void __MoveConstruct(weak_ptr<U>&& move) noexcept
		{
			m_Ptr          = move.m_Ptr;
			m_Control      = move.m_Control;
			move.m_Ptr     = nullptr;
			move.m_Control = nullptr;
		}

		template <class U>
		weak_ptr& __CopyAssign(const weak_ptr<U>& copy) noexcept
		{
			if (m_Control && --m_Control->m_Weak == 0)
				m_Control->DeleteThis();
			m_Ptr     = copy.m_Ptr;
			m_Control = copy.m_Control;
			if (m_Control) m_Control->m_Weak++;
			return *this;
		}

		template <class U>
		weak_ptr& __CopyAssign(const shared_ptr<U>& copy) noexcept
		{
			if (m_Control && --m_Control->m_Weak == 0)
				m_Control->DeleteThis();
			m_Ptr     = copy.m_Ptr;
			m_Control = copy.m_Control;
			if (m_Control) m_Control->m_Weak++;
			return *this;
		}

		template <class U>
		weak_ptr& __MoveAssign(weak_ptr<U>&& move) noexcept
		{
			if (m_Control && --m_Control->m_Weak == 0)
				m_Control->DeleteThis();
			m_Ptr          = move.m_Ptr;
			m_Control      = move.m_Control;
			move.m_Ptr     = nullptr;
			move.m_Control = nullptr;
			return *this;
		}

	private:
		T*           m_Ptr     = nullptr;
		ControlBase* m_Control = nullptr;
	};

	template <class T>
	weak_ptr(smart_pointers::shared_ptr<T>) -> weak_ptr<T>;
} // namespace smart_pointers

namespace std
{
	template <class T>
	void swap(smart_pointers::weak_ptr<T>& lhs, smart_pointers::weak_ptr<T>& rhs) noexcept
	{
		lhs.swap(rhs);
	}
} // namespace std