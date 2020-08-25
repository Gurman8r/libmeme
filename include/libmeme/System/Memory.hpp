#ifndef _ML_MEMORY_HPP_
#define _ML_MEMORY_HPP_

#include <libmeme/System/Export.hpp>
#include <libmeme/Core/Debug.hpp>
#include <libmeme/Core/FlatMap.hpp>
#include <libmeme/Core/Singleton.hpp>

namespace ml
{
	// passthrough for testing an upstream resource
	class test_resource final : public pmr::memory_resource, public non_copyable
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using pointer			= typename byte_t *;
		using const_pointer		= typename byte_t const *;
		using reference			= typename byte_t &;
		using const_reference	= typename byte_t const &;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		explicit test_resource(pmr::memory_resource * u, pointer const b, size_t c) noexcept
			: m_upstream{ u }, m_buffer{ b }, m_capacity{ c }
		{
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

namespace ml
{
	// trackable
	struct trackable;

	// no delete
	struct no_delete { template <class T> void operator()(T *) const noexcept {} };

	// default delete
	template <class ...> struct default_delete;

	// scoped pointer ( std::unique_ptr<T, Dx> )
	template <class T, class Dx = default_delete<T>
	> ML_alias scoped = typename std::unique_ptr<T, Dx>;

	// shared pointer ( std::shared_ptr<T> )
	template <class T
	> ML_alias shared = typename std::shared_ptr<T>;
}

namespace ml
{
	// memory manager singleton
	struct ML_SYSTEM_API memory final : non_copyable
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = pmr::polymorphic_allocator<byte_t>;

		static constexpr size_t max_align{ alignof(max_align_t) };

		struct ML_NODISCARD record final
		{
			size_t index; size_t size; byte_t * addr;

			operator bool() const noexcept { return index && size && addr; }
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit memory(test_resource * res) noexcept;

		~memory() noexcept;

		static memory * const get() noexcept { return s_instance; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto const & allocator() const & noexcept { return m_allocator; }
		
		ML_NODISCARD auto const & records() const & noexcept { return m_records; }
		
		ML_NODISCARD auto const & testres() const & noexcept { return m_testres; }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// malloc
		ML_NODISCARD void * mallocate(size_t size) noexcept
		{
			// allocate the requested bytes
			byte_t * const data{ m_allocator.allocate(size) };

			// create the record
			return m_records.insert(data, { ++m_counter, size, data }).second->addr;
		}

		// calloc
		ML_NODISCARD void * callocate(size_t count, size_t size) noexcept
		{
			// allocate (count * size) zeroed bytes
			return std::memset(mallocate(count * size), 0, count * size);
		}

		// free
		void deallocate(void * addr) noexcept
		{
			// find the record
			if (auto const it{ m_records.find(addr) })
			{
				// free the allocation
				m_allocator.deallocate(it->second->addr, it->second->size);

				// erase the record
				m_records.erase(it->first);
			}
		}

		// realloc
		ML_NODISCARD void * reallocate(void * addr, size_t size) noexcept
		{
			return reallocate(addr, size, size);
		}

		// realloc (sized)
		ML_NODISCARD void * reallocate(void * addr, size_t oldsz, size_t newsz) noexcept
		{
			if (newsz == 0)
			{
				deallocate(addr);

				return nullptr;
			}
			else if (addr == nullptr)
			{
				return mallocate(newsz);
			}
			else if (newsz <= oldsz)
			{
				return addr;
			}
			else
			{
				void * const temp{ mallocate(newsz) };
				if (temp)
				{
					std::memcpy(temp, addr, oldsz);

					deallocate(addr);
				}
				return temp;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		template <class T, class ... Args
		> static T * construct(T * ptr, Args && ... args) noexcept
		{
			return ::new (ptr) T{ ML_forward(args)... };
		}

		template <class T
		> static void destroy(T * ptr) noexcept
		{
			ptr->~T();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void * allocate_bytes(size_t bytes, size_t align = max_align)
		{
			return m_testres->allocate(bytes, align);
		}

		void deallocate_bytes(void * ptr, size_t bytes, size_t align = max_align)
		{
			m_testres->deallocate(ptr, bytes, align);
		}

		template <class T
		> T * allocate_object(size_t count = 1) noexcept
		{
			return (T *)allocate_bytes(count * sizeof(T), alignof(T));
		}

		template <class T
		> void deallocate_object(T * ptr) noexcept
		{
			deallocate_bytes(ptr, sizeof(T), alignof(T));
		}

		template <class T, class ... Args
		> T * new_object(Args && ... args) noexcept
		{
			return construct(allocate_object<T>(), ML_forward(args)...);
		}

		template <class T
		> void delete_object(T * ptr) noexcept
		{
			destroy(ptr);
			deallocate_object(ptr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// allocate scoped
		template <class T, class Dx, class ... Args
		> ML_NODISCARD scoped<T, Dx> alloc_scoped(Args && ... args) noexcept
		{
			return { get()->new_object<T>(ML_forward(args)...), Dx{} };
		}

		// allocate shared
		template <class T, class ... Args
		> ML_NODISCARD shared<T> alloc_shared(Args && ... args) noexcept
		{
			return std::allocate_shared<T>(get()->m_allocator, ML_forward(args)...);
		}

	private:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static memory *			s_instance;		// instance
		allocator_type			m_allocator	{};	// allocator
		size_t					m_counter	{};	// record index
		ds::map<void *, record>	m_records	{};	// memory records
		test_resource *			m_testres	{};	// pointer to test resource

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	static void to_json(json & j, memory::record const & v)
	{
		j["index"] = v.index;
		j["size"] = v.size;
		j["addr"] = (intptr_t)v.addr;
	}

	static void from_json(json const & j, memory::record & v)
	{
		j["index"].get_to(v.index);
		j["size"].get_to(v.size);
		j["addr"].get_to(*(intptr_t *)v.addr);
	}
}

namespace ml
{
	// trackable base
	struct ML_SYSTEM_API trackable
	{
	public:
		virtual ~trackable() noexcept = default;

		ML_NODISCARD void * operator new(size_t size) noexcept { return memory::get()->mallocate(size); }

		ML_NODISCARD void * operator new[](size_t size) noexcept { return memory::get()->mallocate(size); }

		void operator delete(void * addr) noexcept { memory::get()->deallocate(addr); }

		void operator delete[](void * addr) noexcept { memory::get()->deallocate(addr); }
	};
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct default_delete<>
	{
		void operator()(void * addr) const noexcept
		{
			memory::get()->deallocate(addr);
		}
	};

	template <class T> struct default_delete<T>
	{
		void operator()(T * addr) const noexcept
		{
			if constexpr (std::is_base_of_v<trackable, T>)
			{
				T::operator delete(addr);
			}
			else
			{
				memory::get()->deallocate(addr);
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// allocate scoped
	template <class T, class Dx = default_delete<T>, class ... Args
	> ML_NODISCARD auto alloc_scoped(Args && ... args) noexcept
	{
		return memory::get()->alloc_scoped<T, Dx>(ML_forward(args)...);
	}

	// allocate shared
	template <class T, class ... Args
	> ML_NODISCARD auto alloc_shared(Args && ... args) noexcept
	{
		return memory::get()->alloc_shared<T>(ML_forward(args)...);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_HPP_