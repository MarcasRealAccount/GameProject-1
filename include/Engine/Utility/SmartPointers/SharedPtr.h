#pragma once

#include "Engine/Utility/SmartPointers/Control.h"

#include <type_traits>

namespace smart_pointers
{
	template <class T>
	struct weak_ptr;

	template <class T>
	struct shared_ptr
	{
	public:
		using element_type  = std::remove_extent_t<T>;
		using weak_ptr_type = weak_ptr<T>;

		template <class U>
		friend struct shared_ptr;
		template <class U>
		friend struct weak_ptr;

	public:
		constexpr shared_ptr() noexcept = default;
		constexpr shared_ptr(std::nullptr_t) noexcept {}

		template <class U, std::enable_if_t<std::conjunction_v<std::conditional_t<std::is_array_v<T>, CanArrayDelete<U>, CanScalarDelete<U>>, SharedPtrConvertible<U, T>>, int> = 0>
		explicit shared_ptr(U* ptr)
		{
			if constexpr (std::is_array_v<T>)
				__ConstructD(ptr, DefaultDelete<U[]> {});
			else
				__Construct(ptr);
		}

		template <class U, class Deleter, std::enable_if_t<std::conjunction_v<std::is_move_constructible<Deleter>, CanCallFunctionObject<Deleter&, U*&>, SharedPtrConvertible<U, T>>, int> = 0>
		shared_ptr(U* ptr, Deleter deleter)
		{
			__ConstructD(ptr, std::move(deleter));
		}

		template <class Deleter, std::enable_if_t<std::conjunction_v<std::is_move_constructible<Deleter>, CanCallFunctionObject<Deleter&, std::nullptr_t*&>>, int> = 0>
		shared_ptr(std::nullptr_t, Deleter deleter)
		{
			__ConstructD(nullptr, std::move(deleter));
		}

		template <class U, class Deleter, class Allocator, std::enable_if_t<std::conjunction_v<std::is_move_constructible<Deleter>, CanCallFunctionObject<Deleter&, U*&>, SharedPtrConvertible<U, T>>, int> = 0>
		shared_ptr(U* ptr, Deleter deleter, Allocator allocator)
		{
			__ConstructDA(ptr, std::move(deleter), std::move(allocator));
		}

		template <class Deleter, class Allocator, std::enable_if_t<std::conjunction_v<std::is_move_constructible<Deleter>, CanCallFunctionObject<Deleter&, std::nullptr_t*&>>, int> = 0>
		shared_ptr(std::nullptr_t, Deleter deleter, Allocator allocator)
		{
			__ConstructDA(nullptr, std::move(deleter), std::move(allocator));
		}

		template <class U>
		shared_ptr(const shared_ptr<U>& alias, element_type* ptr) noexcept
		{
			__AliasConstruct(alias, ptr);
		}

		template <class U>
		shared_ptr(shared_ptr<U>&& alias, element_type* ptr) noexcept
		{
			__AliasMoveConstruct(std::move(alias), ptr);
		}

		shared_ptr(const shared_ptr& copy) noexcept
		{
			__CopyConstruct(copy);
		}

		shared_ptr(shared_ptr&& move) noexcept
		{
			__MoveConstruct(std::move(move));
		}

		template <class U, std::enable_if_t<SharedPtrConvertible<U, T>::value, int> = 0>
		shared_ptr(const shared_ptr<U>& copy) noexcept
		{
			__CopyConstruct(copy);
		}

		template <class U, std::enable_if_t<SharedPtrConvertible<U, T>::value, int> = 0>
		shared_ptr(shared_ptr<U>&& move) noexcept
		{
			__MoveConstruct(std::move(move));
		}

		template <class U, std::enable_if_t<SharedPtrConvertible<U, T>::value, int> = 0>
		explicit shared_ptr(const weak_ptr<U>& copy) noexcept
		{
			__CopyConstruct(copy);
		}

		~shared_ptr() noexcept
		{
			if (m_Control && --m_Control->m_Strong == 0)
			{
				m_Control->Delete();
				if (--m_Control->m_Weak == 0)
					m_Control->DeleteThis();
			}
		}

		shared_ptr& operator=(const shared_ptr& copy) noexcept
		{
			return __CopyAssign(copy);
		}

		template <class U, std::enable_if_t<SharedPtrConvertible<U, T>::value, int> = 0>
		shared_ptr& operator=(const shared_ptr<U>& copy) noexcept
		{
			return __CopyAssign(copy);
		}

		shared_ptr& operator=(shared_ptr&& move) noexcept
		{
			return __MoveAssign(std::move(move));
		}

		template <class U, std::enable_if_t<SharedPtrConvertible<U, T>::value, int> = 0>
		shared_ptr& operator=(shared_ptr<U>&& move) noexcept
		{
			return __MoveAssign(std::move(move));
		}

		void reset() noexcept
		{
			if (m_Control && --m_Control->m_Strong == 0)
			{
				m_Control->Delete();
				if (--m_Control->m_Weak == 0)
					m_Control->DeleteThis();
			}
			m_Ptr     = nullptr;
			m_Control = nullptr;
		}

		template <class U>
		void reset(U* ptr)
		{
			if (m_Control && --m_Control->m_Strong == 0)
			{
				m_Control->Delete();
				if (--m_Control->m_Weak == 0)
					m_Control->DeleteThis();
			}
			__Construct(ptr);
		}

		template <class U, class Deleter>
		void reset(U* ptr, Deleter deleter)
		{
			if (m_Control && --m_Control->m_Strong == 0)
			{
				m_Control->Delete();
				if (--m_Control->m_Weak == 0)
					m_Control->DeleteThis();
			}
			__ConstructD(ptr, std::move(deleter));
		}

		template <class U, class Deleter, class Allocator>
		void reset(U* ptr, Deleter deleter, Allocator allocator)
		{
			if (m_Control && --m_Control->m_Strong == 0)
			{
				m_Control->Delete();
				if (--m_Control->m_Weak == 0)
					m_Control->DeleteThis();
			}
			__ConstructDA(ptr, std::move(deleter), std::move(allocator));
		}

		void swap(shared_ptr& other) noexcept
		{
			std::swap(m_Ptr, other.m_Ptr);
			std::swap(m_Control, other.m_Control);
		}

		element_type* get() const noexcept
		{
			return m_Ptr;
		}

		template <class U = T, std::enable_if_t<!std::disjunction_v<std::is_array<U>, std::is_void<U>>, int> = 0>
		U& operator*() const noexcept
		{
			return *m_Ptr;
		}

		template <class U = T, std::enable_if_t<!std::is_array_v<U>, int> = 0>
		U* operator->() const noexcept
		{
			return m_Ptr;
		}

		template <class U = T, class Elem = element_type, std::enable_if_t<std::is_array_v<U>, int> = 0>
		Elem& operator[](ptrdiff_t index) const noexcept
		{
			return m_Ptr[index];
		}

		long use_count() const noexcept
		{
			if (m_Control) return m_Control->StrongCount();
			return 0;
		}

		bool unique() const noexcept
		{
			return use_count() == 1;
		}

		explicit operator bool() const noexcept
		{
			return m_Ptr != nullptr;
		}

		template <class U>
		bool owner_before(const shared_ptr<U>& other) const noexcept
		{
			return m_Control != other.m_Control;
		}

	private:
		template <class U>
		void __CopyConstruct(const shared_ptr<U>& copy) noexcept
		{
			m_Ptr     = copy.m_Ptr;
			m_Control = copy.m_Control;
			if (m_Control) m_Control->m_Strong++;
		}

		template <class U>
		void __CopyConstruct(const weak_ptr<U>& copy) noexcept
		{
			if (!copy.expired())
			{
				m_Ptr     = copy.m_Ptr;
				m_Control = copy.m_Control;
				if (m_Control) m_Control->m_Strong++;
			}
		}

		template <class U>
		void __MoveConstruct(shared_ptr<U>&& move) noexcept
		{
			m_Ptr          = move.m_Ptr;
			m_Control      = move.m_Control;
			move.m_Ptr     = nullptr;
			move.m_Control = nullptr;
		}

		template <class U>
		void __AliasConstruct(const shared_ptr<U>& alias, element_type* ptr) noexcept
		{
			m_Ptr     = ptr;
			m_Control = alias.m_Control;
			if (m_Control) m_Control->m_Strong++;
		}

		template <class U>
		shared_ptr& __CopyAssign(const shared_ptr<U>& copy) noexcept
		{
			if (m_Control && --m_Control->m_Strong == 0)
			{
				m_Control->Delete();
				if (--m_Control->m_Weak == 0)
					m_Control->DeleteThis();
			}
			m_Ptr     = copy.m_Ptr;
			m_Control = copy.m_Control;
			if (m_Control) m_Control->m_Strong++;
			return *this;
		}

		template <class U>
		shared_ptr& __MoveAssign(shared_ptr<U>&& move) noexcept
		{
			if (m_Control && --m_Control->m_Strong == 0)
			{
				m_Control->Delete();
				if (--m_Control->m_Weak == 0)
					m_Control->DeleteThis();
			}
			m_Ptr          = move.m_Ptr;
			m_Control      = move.m_Control;
			move.m_Ptr     = nullptr;
			move.m_Control = nullptr;
			return *this;
		}

		template <class U>
		shared_ptr& __Construct(U ptr)
		{
			return __Set(ptr, new Control<U>(ptr));
		}

		template <class U, class Deleter>
		shared_ptr& __ConstructD(U ptr, Deleter deleter)
		{
			return __Set(ptr, new ControlD<U, Deleter>(ptr, std::move(deleter)));
		}

		template <class U, class Deleter, class Allocator>
		shared_ptr& __ConstructDA(U ptr, Deleter deleter, Allocator allocator)
		{
			using ControlT     = ControlDA<U, Deleter, Allocator>;
			using ControlAlloc = RebindAllocator<Allocator, ControlT>;

			ControlAlloc alloc(std::move(allocator));
			ControlT*    control = alloc.allocate(1);
			new (control) ControlT(ptr, std::move(deleter), std::move(allocator));
			return __Set(ptr, control);
		}

		template <class... Params>
		shared_ptr& __ConstructI(Params&&... args)
		{
			ControlI<element_type>* control = new ControlI<element_type>(std::forward<Params>(args)...);
			return __Set(control->GetPtr(), control);
		}

		template <class Allocator, class... Params>
		shared_ptr& __ConstructIA(Allocator allocator, Params&&... args)
		{
			using ControlT     = ControlIA<T, Allocator>;
			using ControlAlloc = RebindAllocator<Allocator, ControlT>;

			ControlAlloc alloc(std::move(allocator));
			ControlT*    control = alloc.allocate(1);
			new (control) ControlT(std::move(allocator), std::forward<Params>(args)...);
			return __Set(control->GetPtr(), control);
		}

		template <class U, class ControlT>
		shared_ptr& __Set(U* const ptr, ControlT* const control)
		{
			m_Ptr     = ptr;
			m_Control = control;
			return *this;
		}

		template <class ControlT>
		shared_ptr& __Set(std::nullptr_t, ControlT* const control)
		{
			m_Ptr     = nullptr;
			m_Control = control;
			return *this;
		}

	private:
		element_type* m_Ptr     = nullptr;
		ControlBase*  m_Control = nullptr;

	public:
		template <class U, class... Params>
		friend std::enable_if_t<!std::is_array_v<U>, shared_ptr<U>> make_shared(Params&&... args);

		template <class U>
		friend std::enable_if_t<IsUnboundedArray_v<U>, shared_ptr<U>> make_shared(size_t N);

		template <class U>
		friend std::enable_if_t<IsUnboundedArray_v<U>, shared_ptr<U>> make_shared(size_t N, const std::remove_extent_t<U>& u);

		template <class U>
		friend std::enable_if_t<IsBoundedArray_v<U>, shared_ptr<U>> make_shared();

		template <class U>
		friend std::enable_if_t<IsBoundedArray_v<U>, shared_ptr<U>> make_shared(const std::remove_extent_t<U>& u);

		template <class U, class Allocator, class... Params>
		friend std::enable_if_t<!std::is_array_v<U>, shared_ptr<U>> allocate_shared(Allocator allocator, Params&&... args);

		template <class U, class Allocator>
		friend std::enable_if_t<IsUnboundedArray_v<U>, shared_ptr<U>> allocate_shared(size_t N, Allocator allocator);

		template <class U, class Allocator>
		friend std::enable_if_t<IsUnboundedArray_v<U>, shared_ptr<U>> allocate_shared(size_t N, const std::remove_extent_t<U>& u, Allocator allocator);

		template <class U, class Allocator>
		friend std::enable_if_t<IsBoundedArray_v<U>, shared_ptr<U>> allocate_shared(Allocator allocator);

		template <class U, class Allocator>
		friend std::enable_if_t<IsBoundedArray_v<U>, shared_ptr<U>> allocate_shared(const std::remove_extent_t<U>& u, Allocator allocator);

		template <class Deleter, class U>
		friend Deleter* get_deleter(const shared_ptr<U>& p) noexcept;
	};

	template <class T>
	shared_ptr(weak_ptr<T>) -> shared_ptr<T>;

	template <class T, class... Params>
	std::enable_if_t<!std::is_array_v<T>, shared_ptr<T>> make_shared(Params&&... args)
	{
		return shared_ptr<T>().__ConstructI(std::forward<Params>(args)...);
	}

	template <class T>
	std::enable_if_t<IsUnboundedArray_v<T>, shared_ptr<T>> make_shared(size_t N)
	{
		return shared_ptr<T>(new std::remove_extent_t<T>[N]);
	}

	template <class T>
	std::enable_if_t<IsUnboundedArray_v<T>, shared_ptr<T>> make_shared(size_t N, const std::remove_extent_t<T>& u)
	{
		return shared_ptr<T>(new std::remove_extent_t<T>[N] { u });
	}

	template <class T>
	std::enable_if_t<IsBoundedArray_v<T>, shared_ptr<T>> make_shared()
	{
		return shared_ptr<T>().__ConstructI();
	}

	template <class T>
	std::enable_if_t<IsBoundedArray_v<T>, shared_ptr<T>> make_shared(const std::remove_extent_t<T>& u)
	{
		return shared_ptr<T>().__ConstructI(u);
	}

	template <class T, class Allocator, class... Params>
	std::enable_if_t<!std::is_array_v<T>, shared_ptr<T>> allocate_shared(Allocator allocator, Params&&... args)
	{
		return shared_ptr<T>().__ConstructIA(std::move(allocator), std::forward<Params>(args)...);
	}

	template <class T, class Allocator>
	std::enable_if_t<IsUnboundedArray_v<T>, shared_ptr<T>> allocate_shared(size_t N, Allocator allocator)
	{
		using BaseT        = std::remove_extent_t<T>;
		using DataPtrAlloc = __RebindAllocator<Allocator, BaseT>;
		using Deletor      = __DeleteAllocator<T, Allocator>;

		shared_ptr<T> ptr;
		DataPtrAlloc  dataPtrAlloc(allocator);
		BaseT*        dataPtr = dataPtrAlloc.allocate(N);
		new (dataPtr) BaseT[N];
		return ptr.__ConstructDA<BaseT*, Deletor, Allocator>(dataPtr, Deletor(std::move(allocator), N), std::move(allocator));
	}

	template <class T, class Allocator>
	std::enable_if_t<IsUnboundedArray_v<T>, shared_ptr<T>> allocate_shared(size_t N, const std::remove_extent_t<T>& u, Allocator allocator)
	{
		using BaseT        = std::remove_extent_t<T>;
		using DataPtrAlloc = __RebindAllocator<Allocator, BaseT>;
		using Deletor      = __DeleteAllocator<T, Allocator>;

		shared_ptr<T> ptr;
		DataPtrAlloc  dataPtrAlloc(allocator);
		BaseT*        dataPtr = dataPtrAlloc.allocate(N);
		new (dataPtr) BaseT[N] { u };
		return ptr.__ConstructDA<BaseT*, Deletor, Allocator>(dataPtr, Deletor(allocator, N), std::move(allocator));
	}

	template <class T, class Allocator>
	std::enable_if_t<IsBoundedArray_v<T>, shared_ptr<T>> allocate_shared(Allocator allocator)
	{
		return shared_ptr<T>().__ConstructIA(std::move(allocator));
	}

	template <class T, class Allocator>
	std::enable_if_t<IsBoundedArray_v<T>, shared_ptr<T>> allocate_shared(const std::remove_extent_t<T>& u, Allocator allocator)
	{
		return shared_ptr<T>().__ConstructIA(std::move(allocator), u);
	}

	template <class T, class U>
	shared_ptr<T> static_pointer_cast(const shared_ptr<U>& r) noexcept
	{
		return shared_ptr<T>(r, static_cast<typename shared_ptr<T>::element_type*>(r.get()));
	}

	template <class T, class U>
	shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& r) noexcept
	{
		return shared_ptr<T>(r, dynamic_cast<typename shared_ptr<T>::element_type*>(r.get()));
	}

	template <class T, class U>
	shared_ptr<T> const_pointer_cast(const shared_ptr<U>& r) noexcept
	{
		return shared_ptr<T>(r, const_cast<typename shared_ptr<T>::element_type*>(r.get()));
	}

	template <class T, class U>
	shared_ptr<T> reinterpret_pointer_cast(const shared_ptr<U>& r) noexcept
	{
		return shared_ptr<T>(r, reinterpret_cast<typename shared_ptr<T>::element_type*>(r.get()));
	}

	template <class Deleter, class T>
	Deleter* get_deleter(const shared_ptr<T>& p) noexcept
	{
		if (p.m_Control) return reinterpret_cast<Deleter*>(p.m_Control->GetDeleter());
		return nullptr;
	}

	template <class T, class U>
	bool operator==(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) noexcept
	{
		return lhs.get() == rhs.get();
	}

	template <class T, class U>
	bool operator!=(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) noexcept
	{
		return !(lhs == rhs);
	}

	template <class T, class U>
	bool operator<(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) noexcept
	{
		return std::less<std::common_type_t<shared_ptr<T>::element_type*, shared_ptr<U>::element_type*>>()(lhs.get(), rhs.get());
	}

	template <class T, class U>
	bool operator>(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) noexcept
	{
		return rhs < lhs;
	}

	template <class T, class U>
	bool operator<=(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) noexcept
	{
		return !(rhs < lhs);
	}

	template <class T, class U>
	bool operator>=(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) noexcept
	{
		return !(lhs < rhs);
	}

	template <class T>
	bool operator==(const shared_ptr<T>& lhs, std::nullptr_t) noexcept
	{
		return !lhs.get();
	}

	template <class T>
	bool operator==(std::nullptr_t, const shared_ptr<T>& rhs) noexcept
	{
		return !rhs.get();
	}

	template <class T>
	bool operator!=(const shared_ptr<T>& lhs, std::nullptr_t) noexcept
	{
		return (bool) lhs;
	}

	template <class T>
	bool operator!=(std::nullptr_t, const shared_ptr<T>& rhs) noexcept
	{
		return (bool) rhs;
	}

	template <class T>
	bool operator<(const shared_ptr<T>& lhs, std::nullptr_t) noexcept
	{
		return std::less<shared_ptr<T>::element_type*>()(lhs.get(), nullptr);
	}

	template <class T>
	bool operator<(std::nullptr_t, const shared_ptr<T>& rhs) noexcept
	{
		return std::less<shared_ptr<T>::element_type*>()(nullptr, rhs.get());
	}

	template <class T>
	bool operator>(const shared_ptr<T>& lhs, std::nullptr_t) noexcept
	{
		return nullptr < lhs;
	}

	template <class T>
	bool operator>(std::nullptr_t, const shared_ptr<T>& rhs) noexcept
	{
		return lhs < nullptr;
	}

	template <class T>
	bool operator<=(const shared_ptr<T>& lhs, std::nullptr_t) noexcept
	{
		return !(nullptr < lhs);
	}

	template <class T>
	bool operator<=(std::nullptr_t, const shared_ptr<T>& rhs) noexcept
	{
		return !(rhs < nullptr);
	}

	template <class T>
	bool operator>=(const shared_ptr<T>& lhs, std::nullptr_t) noexcept
	{
		return !(lhs < nullptr);
	}

	template <class T>
	bool operator>=(std::nullptr_t, const shared_ptr<T>& rhs) noexcept
	{
		return !(nullptr < rhs);
	}

	template <class T, class U, class V>
	std::basic_ostream<U, V>& operator<<(std::basic_ostream<U, V>& lhs, const shared_ptr<T>& rhs) noexcept
	{
		return lhs << rhs.get();
	}
} // namespace smart_pointers

namespace std
{
	template <class T>
	void swap(smart_pointers::shared_ptr<T>& lhs, smart_pointers::shared_ptr<T>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	template <class T>
	struct hash<smart_pointers::shared_ptr<T>>
	{
		void operator()(const smart_pointers::shared_ptr<T>& ptr) noexcept
		{
			return hash<typename smart_pointers::shared_ptr<T>::element_type*>()(ptr.get());
		}
	};
} // namespace std
