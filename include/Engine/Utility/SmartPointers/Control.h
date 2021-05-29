#pragma once

#include <memory>
#include <type_traits>

namespace smart_pointers
{
	template <class Allocator, class ValueType>
	using RebindAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<ValueType>;

	template <class T>
	struct IsUnboundedArray : public std::false_type
	{
	};
	template <class T>
	struct IsUnboundedArray<T[]> : public std::true_type
	{
	};
	template <class T>
	static constexpr const bool IsUnboundedArray_v = IsUnboundedArray<T>::value;

	template <class T>
	struct IsBoundedArray : public std::false_type
	{
	};
	template <class T, size_t N>
	struct IsBoundedArray<T[N]> : public std::true_type
	{
	};
	template <class T>
	static constexpr const bool IsBoundedArray_v = IsBoundedArray<T>::value;

	template <class T, class = void>
	struct CanArrayDelete : public std::false_type
	{
	};
	template <class T>
	struct CanArrayDelete<T, std::void_t<decltype(delete[] std::declval<T*>())>> : public std::true_type
	{
	};

	template <class T, class = void>
	struct CanScalarDelete : public std::false_type
	{
	};
	template <class T>
	struct CanScalarDelete<T, std::void_t<decltype(delete std::declval<T*>())>> : public std::true_type
	{
	};

	template <class Function, class Arg, class = void>
	struct CanCallFunctionObject : public std::false_type
	{
	};
	template <class Function, class Arg>
	struct CanCallFunctionObject<Function, Arg, std::void_t<decltype(std::declval<Function>()(std::declval<Arg>()))>> : public std::true_type
	{
	};

	template <class T, class U>
	struct SharedPtrConvertible : public std::is_convertible<T*, U*>::type
	{
	};
	template <class T, class U>
	struct SharedPtrConvertible<T, U[]> : public std::is_convertible<T (*)[], U (*)[]>::type
	{
	};
	template <class T, class U, size_t N>
	struct SharedPtrConvertible<T, U[N]> : public std::is_convertible<T (*)[N], U (*)[N]>::type
	{
	};

	template <typename T>
	struct DefaultDelete
	{
	public:
		void operator()(T* ptr) noexcept
		{
			if constexpr (std::is_array_v<T>)
				delete[] ptr;
			else
				delete ptr;
		}
	};

	template <typename T, class Allocator>
	struct DeleteAllocator
	{
	public:
		using allocator_type = RebindAllocator<Allocator, T>;

	public:
		DeleteAllocator(const Allocator& allocator) noexcept
		    : m_Allocator(allocator) {}

		void operator()(T* ptr)
		{
			m_Allocator.destroy(ptr);
			m_Allocator.deallocate(ptr, 1);
		}

	private:
		allocator_type m_Allocator;
	};

	template <typename T, class Allocator>
	struct DeleteAllocator<T[], Allocator>
	{
	public:
		using element_type   = std::remove_extent_t<T>;
		using allocator_type = RebindAllocator<Allocator, element_type>;

	public:
		DeleteAllocator(const Allocator& allocator, size_t N) noexcept
		    : m_Allocator(std::move(allocator)), m_N(N) {}

		void operator()(element_type* ptr)
		{
			for (size_t i = 0; i < m_N; i++)
				m_Allocator.destroy(ptr + i);
			m_Allocator.deallocate(ptr, m_N);
		}

	private:
		allocator_type m_Allocator;
		size_t         m_N;
	};

	struct ControlBase
	{
	public:
		template <class T>
		friend struct weak_ptr;
		template <class T>
		friend struct shared_ptr;

	public:
		ControlBase(const ControlBase&) = delete;
		ControlBase(ControlBase&&)      = delete;
		ControlBase& operator=(const ControlBase&) = delete;
		ControlBase& operator=(ControlBase&&) = delete;

		virtual void* GetDeleter() const noexcept
		{
			return nullptr;
		}

		long StrongCount() const noexcept
		{
			return m_Strong;
		}

		long WeakCount() const noexcept
		{
			return m_Weak;
		}

	protected:
		constexpr ControlBase() noexcept = default;
		virtual ~ControlBase() noexcept  = default;

	private:
		virtual void Delete() noexcept     = 0;
		virtual void DeleteThis() noexcept = 0;

	private:
		long m_Strong = 1;
		long m_Weak   = 1;
	};

	template <class T>
	struct Control final : public ControlBase
	{
	public:
		explicit Control(T ptr)
		    : m_Ptr(ptr) {}

		Control(const Control&) = delete;
		Control(Control&&)      = delete;
		Control& operator=(const Control&) = delete;
		Control& operator=(Control&&) = delete;

	protected:
		virtual ~Control() noexcept = default;

	private:
		virtual void Delete() noexcept override
		{
			if constexpr (!std::is_same_v<T, std::nullptr_t>)
			{
				if (m_Ptr)
				{
					delete m_Ptr;
					m_Ptr = nullptr;
				}
			}
		}

		virtual void DeleteThis() noexcept override
		{
			delete this;
		}

	private:
		T m_Ptr;
	};

	template <class T, class Deleter>
	struct ControlD final : public ControlBase
	{
	public:
		ControlD(T ptr, Deleter deleter)
		    : m_Ptr(ptr), m_Deleter(std::move(deleter)) {}

		ControlD(const ControlD&) = delete;
		ControlD(ControlD&&)      = delete;
		ControlD& operator=(const ControlD&) = delete;
		ControlD& operator=(ControlD&&) = delete;

		virtual void* GetDeleter() const noexcept override
		{
			return const_cast<void*>(reinterpret_cast<const void*>(&m_Deleter));
		}

	protected:
		virtual ~ControlD() noexcept = default;

	private:
		virtual void Delete() noexcept override
		{
			m_Deleter(m_Ptr);
			m_Ptr = nullptr;
		}

		virtual void DeleteThis() noexcept override
		{
			delete this;
		}

	private:
		T       m_Ptr;
		Deleter m_Deleter;
	};

	template <class T, class Deleter, class Allocator>
	struct ControlDA final : public ControlBase
	{
	public:
		using allocator_type = RebindAllocator<Allocator, ControlDA<T, Deleter, Allocator>>;

	public:
		ControlDA(T ptr, Deleter deleter, Allocator allocator)
		    : m_Ptr(ptr), m_Deleter(std::move(deleter)), m_Allocator(std::move(allocator)) {}

		ControlDA(const ControlDA&) = delete;
		ControlDA(ControlDA&&)      = delete;
		ControlDA& operator=(const ControlDA&) = delete;
		ControlDA& operator=(ControlDA&&) = delete;

		virtual void* GetDeleter() const noexcept override
		{
			return const_cast<void*>(reinterpret_cast<const void*>(&m_Deleter));
		}

	protected:
		virtual ~ControlDA() noexcept = default;

	private:
		virtual void Delete() noexcept override
		{
			m_Deleter(m_Ptr);
			m_Ptr = nullptr;
		}

		virtual void DeleteThis() noexcept override
		{
			m_Allocator.destroy(this);
			m_Allocator.deallocate(this, 1);
		}

	private:
		T              m_Ptr;
		Deleter        m_Deleter;
		allocator_type m_Allocator;
	};

	template <class T>
	struct ControlI final : public ControlBase
	{
	public:
		template <class... Params>
		explicit ControlI(Params&&... args)
		    : m_Data(std::forward<Params>(args)...)
		{
		}

		ControlI(const ControlI&) = delete;
		ControlI(ControlI&&)      = delete;
		ControlI& operator=(const ControlI&) = delete;
		ControlI& operator=(ControlI&&) = delete;

		inline T* GetPtr() const noexcept
		{
			return const_cast<T*>(&m_Data);
		}

	protected:
		virtual ~ControlI() noexcept {}

	private:
		virtual void Delete() noexcept override {}
		virtual void DeleteThis() noexcept override
		{
			delete this;
		}

	private:
		T m_Data;
	};

	template <class T, size_t N>
	struct ControlI<T[N]> final : public ControlBase
	{
	public:
		ControlI() = default;
		explicit ControlI(const T& u)
		{
			for (size_t i = 0; i < N; i++)
				m_Datas[i] = u;
		}

		ControlI(const ControlI&) = delete;
		ControlI(ControlI&&)      = delete;
		ControlI& operator=(const ControlI&) = delete;
		ControlI& operator=(ControlI&&) = delete;

		inline T* GetPtr() const noexcept
		{
			return const_cast<T*>(&m_Datas);
		}

	protected:
		virtual ~ControlI() noexcept {}

	private:
		virtual void Delete() noexcept override {}
		virtual void DeleteThis() noexcept override
		{
			delete this;
		}

	private:
		T m_Datas[N];
	};

	template <class T, class Allocator>
	struct ControlIA final : public ControlBase
	{
	public:
		using allocator_type = RebindAllocator<Allocator, ControlIA<T, Allocator>>;

	public:
		template <class... Params>
		explicit ControlIA(Allocator allocator, Params&&... args)
		    : m_Data(std::forward<Params>(args)...), m_Allocator(std::move(allocator))
		{
		}

		ControlIA(const ControlIA&) = delete;
		ControlIA(ControlIA&&)      = delete;
		ControlIA& operator=(const ControlIA&) = delete;
		ControlIA& operator=(ControlIA&&) = delete;

		inline T* GetPtr() const noexcept
		{
			return const_cast<T*>(&m_Data);
		}

	protected:
		virtual ~ControlIA() noexcept {}

	private:
		virtual void Delete() noexcept override {}
		virtual void DeleteThis() noexcept override
		{
			m_Allocator.destroy(this);
			m_Allocator.deallocate(this, 1);
		}

	private:
		T              m_Data;
		allocator_type m_Allocator;
	};

	template <class T, class Allocator, size_t N>
	struct ControlIA<T[N], Allocator> final : public ControlBase
	{
	public:
		using allocator_type = RebindAllocator<Allocator, ControlIA<T[N], Allocator>>;

	public:
		ControlIA(Allocator allocator)
		    : m_Allocator(std::move(allocator)) {}
		explicit ControlIA(Allocator allocator, const T& u)
		    : m_Allocator(std::move(allocator))
		{
			for (size_t i = 0; i < N; i++)
				m_Datas[i] = u;
		}

		ControlIA(const ControlIA&) = delete;
		ControlIA(ControlIA&&)      = delete;
		ControlIA& operator=(const ControlIA&) = delete;
		ControlIA& operator=(ControlIA&&) = delete;

		inline T* GetPtr() const noexcept
		{
			return const_cast<T*>(m_Datas);
		}

	protected:
		virtual ~ControlIA() noexcept {}

	private:
		virtual void Delete() noexcept override {}
		virtual void DeleteThis() noexcept override
		{
			m_Allocator.destroy(this);
			m_Allocator.deallocate(this, 1);
		}

	private:
		T              m_Datas[N];
		allocator_type m_Allocator;
	};
} // namespace smart_pointers