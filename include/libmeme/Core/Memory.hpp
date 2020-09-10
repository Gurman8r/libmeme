#ifndef _ML_MEMORY_HPP_
#define _ML_MEMORY_HPP_

#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/FlatMap.hpp>
#include <libmeme/Core/Singleton.hpp>

// testres
namespace ml
{
	// proxy for testing an upstream resource
	class passthrough_resource final : public pmr::memory_resource, public non_copyable
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using pointer			= typename byte_t *;
		using const_pointer		= typename byte_t const *;
		using reference			= typename byte_t &;
		using const_reference	= typename byte_t const &;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		explicit passthrough_resource(pmr::memory_resource * u, pointer const b, size_t c) noexcept
			: m_upstream{ u }, m_buffer{ b }, m_capacity{ c }
		{
			ML_assert("invalid passthrough_resource parameters" && u && b && c);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		pointer buffer() noexcept { return m_buffer; }

		const_pointer buffer() const noexcept { return m_buffer; }

		pmr::memory_resource * upstream() const noexcept { return m_upstream; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool use_default() const noexcept { return (this == pmr::get_default_resource()); }

		bool in_range(void * addr) const noexcept { return ((pointer)addr >= begin()) && ((pointer)addr < end()); }

		bool is_valid_size() const noexcept { return (0 < m_capacity); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		size_t base_addr() const noexcept { return (size_t)m_buffer; }

		size_t capacity() const noexcept { return m_capacity; }

		size_t used_bytes() const noexcept { return m_bytes_used; }

		size_t free_bytes() const noexcept { return m_capacity - m_bytes_used; }

		float_t fraction_used() const noexcept { return (float_t)m_bytes_used / (float_t)m_capacity; }

		size_t num_allocations() const noexcept { return m_num_alloc; }

		float_t percent_used() const noexcept { return fraction_used() * 100.f; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		reference front() & noexcept { return m_buffer[0]; }

		const_reference front() const & noexcept { return m_buffer[0]; }

		reference back() & noexcept { return m_buffer[m_capacity - 1]; }

		const_reference back() const & noexcept { return m_buffer[m_capacity - 1]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		pointer begin() noexcept { return m_buffer; }

		const_pointer begin() const noexcept { return m_buffer; }

		const_pointer cbegin() const noexcept { return begin(); }

		pointer end() noexcept { return m_buffer + m_capacity; }

		const_pointer end() const noexcept { return m_buffer + m_capacity; }

		const_pointer cend() const noexcept { return end(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		void * do_allocate(size_t bytes, size_t align) override
		{
			++m_num_alloc;
			m_bytes_used += bytes;
			return m_upstream->allocate(bytes, align);
		}

		void do_deallocate(void * ptr, size_t bytes, size_t align) override
		{
			--m_num_alloc;
			m_bytes_used -= bytes;
			return m_upstream->deallocate(ptr, bytes, align);
		}

		bool do_is_equal(pmr::memory_resource const & value) const noexcept override
		{
			return m_upstream->is_equal(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		pmr::memory_resource * m_upstream;
		pointer const m_buffer;
		size_t const m_capacity;

		size_t m_num_alloc{};
		size_t m_bytes_used{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// types
namespace ml
{
	// trackable
	struct trackable;

	// no delete
	struct no_delete final
	{
		template <class T> void operator()(T *) const noexcept {}
	};

	// default delete
	template <class ...> struct default_delete;

	// scoped pointer class ( std::unique_ptr<T, Dx> )
	template <class T, class Dx = default_delete<T>
	> ML_alias scoped = typename std::unique_ptr<T, Dx>;

	// manual pointer class ( std::unique_ptr<T, no_delete> )
	template <class T
	> ML_alias manual = typename scoped<T, no_delete>;

	// shared pointer class ( std::shared_ptr<T> )
	template <class T
	> ML_alias shared = typename std::shared_ptr<T>;

	// unowned pointer class ( std::weak_ptr<T> )
	template <class T
	> ML_alias unown = typename std::weak_ptr<T>;
}

// memory
namespace ml
{
	// memory manager
	struct ML_CORE_API memory final : non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		struct ML_NODISCARD record final
		{
			size_t index; byte_t * addr; size_t count; size_t size;

			operator bool() const noexcept { return index && addr && count && size; }
		};

		using record_map = typename ds::map<void *, record>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit memory(passthrough_resource * res) noexcept;

		explicit memory(pmr::memory_resource * res) : memory{
			reinterpret_cast<passthrough_resource *>(res)
		}
		{
		}

		~memory() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static auto get() noexcept -> memory * { return g_mem; }

		ML_NODISCARD static auto allocator() noexcept -> allocator_type { return g_mem->m_alloc; }

		ML_NODISCARD static auto counter() noexcept -> size_t { return g_mem->m_counter; }

		ML_NODISCARD static auto records() noexcept -> record_map const & { return g_mem->m_records; }
		
		ML_NODISCARD static auto resource() noexcept -> passthrough_resource * { return g_mem->m_resource; }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// malloc
		ML_NODISCARD static void * allocate(size_t size) noexcept
		{
			return memory::allocate(1, size);
		}

		// calloc
		ML_NODISCARD static void * allocate(size_t count, size_t size) noexcept
		{
			ML_assert(g_mem);
			return g_mem->do_allocate(count, size);
		}

		// free
		static void deallocate(void * addr) noexcept
		{
			ML_assert(g_mem);
			g_mem->do_deallocate(addr);
		}

		// realloc
		static void * reallocate(void * addr, size_t size) noexcept
		{
			return memory::reallocate(addr, size, size);
		}

		// realloc (sized)
		static void * reallocate(void * addr, size_t oldsz, size_t newsz) noexcept
		{
			if (newsz == 0)				{ memory::deallocate(addr); return nullptr; }
			else if (addr == nullptr)	{ return memory::allocate(newsz); }
			else if (newsz <= oldsz)	{ return addr; }
			else
			{
				auto const temp{ memory::allocate(newsz) };
				if (temp)
				{
					std::memcpy(temp, addr, oldsz);
					memory::deallocate(addr);
				}
				return temp;
			}
		}

		// allocate object
		template <class T
		> ML_NODISCARD static T * allocate_object(size_t count = 1) noexcept
		{
			return (T *)memory::allocate(count, sizeof(T));
		}

		// deallocate object
		template <class T
		> static void deallocate_object(T * addr) noexcept
		{
			memory::deallocate(addr);
		}

		// new object
		template <class T, class ... Args
		> ML_NODISCARD static T * new_object(Args && ... args) noexcept
		{
			return util::construct(memory::allocate_object<T>(), ML_forward(args)...);
		}

		// delete object
		template <class T
		> static void delete_object(T * addr) noexcept
		{
			util::destruct(addr);
			memory::deallocate_object(addr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		void * do_allocate(size_t count, size_t size) noexcept
		{
			auto const temp{ m_alloc.allocate(count * size) };

			return m_records.insert(temp, { ++m_counter, temp, count, size }).second->addr;
		}

		void do_deallocate(void * addr) noexcept
		{
			if (auto const it{ m_records.find(addr) })
			{
				m_alloc.deallocate(it->second->addr, it->second->count * it->second->size);

				m_records.erase(it->first);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static memory *					g_mem		; // instance
		allocator_type					m_alloc		; // allocator
		size_t							m_counter	; // counter
		record_map						m_records	; // records
		passthrough_resource * const	m_resource	; // resource

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	static void to_json(json & j, memory::record const & v)
	{
		j["index"	] = v.index;
		j["addr"	] = (intptr_t)v.addr;
		j["count"	] = v.count;
		j["size"	] = v.size;
	}

	static void from_json(json const & j, memory::record & v)
	{
		j["index"	].get_to(v.index);
		j["addr"	].get_to(*(intptr_t *)v.addr);
		j["count"	].get_to(v.count);
		j["size"	].get_to(v.size);
	}
}

// c-like interface
namespace ml
{
	inline void * ml_malloc(size_t size) noexcept
	{
		return memory::allocate(size);
	}

	inline void * ml_calloc(size_t count, size_t size) noexcept
	{
		return memory::allocate(count, size);
	}

	inline void ml_free(void * addr) noexcept
	{
		memory::deallocate(addr);
	}

	inline void * ml_realloc(void * addr, size_t size) noexcept
	{
		return memory::reallocate(addr, size);
	}

	inline void * ml_realloc(void * addr, size_t oldsz, size_t newsz) noexcept
	{
		return memory::reallocate(addr, oldsz, newsz);
	}
}

// trackable
namespace ml
{
	// trackable base
	struct ML_CORE_API trackable
	{
		virtual ~trackable() noexcept = default;

		ML_NODISCARD void * operator new(size_t size) noexcept { return ml_malloc(size); }

		ML_NODISCARD void * operator new[](size_t size) noexcept { return ml_malloc(size); }

		void operator delete(void * addr) noexcept { ml_free(addr); }

		void operator delete[](void * addr) noexcept { ml_free(addr); }
	};
}

// impl
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct default_delete<>
	{
		void operator()(void * addr) const noexcept
		{
			memory::deallocate(addr);
		}
	};

	template <class T> struct default_delete<T>
	{
		void operator()(T * addr) const noexcept
		{
			memory::delete_object<T>(addr);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// allocate scoped
	template <class T, class Dx = default_delete<T>, class ... Args
	> ML_NODISCARD scoped<T, Dx> alloc_scoped(Args && ... args) noexcept
	{
		return { memory::new_object<T>(ML_forward(args)...), Dx{} };
	}

	// allocate shared
	template <class T, class ... Args
	> ML_NODISCARD shared<T> alloc_shared(Args && ... args) noexcept
	{
		return std::allocate_shared<T>(memory::allocator(), ML_forward(args)...);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_HPP_