#ifndef _ML_MEMORY_HPP_
#define _ML_MEMORY_HPP_

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
	struct no_delete final
	{
		template <class T> void operator()(T *) const noexcept {}
	};

	// default delete
	template <class ...> struct default_delete;

	// scoped pointer ( std::unique_ptr<T, Dx> )
	template <class T, class Dx = default_delete<T>
	> ML_alias scoped = typename std::unique_ptr<T, Dx>;

	// manual pointer ( std::unique_ptr<T, no_delete> )
	template <class T
	> ML_alias manual = typename scoped<T, no_delete>;

	// shared pointer ( std::shared_ptr<T> )
	template <class T
	> ML_alias shared = typename std::shared_ptr<T>;

	// weak pointer ( std::weak_ptr<T> )
	template <class T
	> ML_alias weakref = typename std::weak_ptr<T>;
}

namespace ml
{
	// memory manager singleton
	struct ML_CORE_API memory final : non_copyable
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		static constexpr size_t max_align{ alignof(max_align_t) };

		struct ML_NODISCARD record final
		{
			byte_t * addr; size_t index; size_t size;

			operator bool() const noexcept { return addr && index && size; }
		};

		using record_table = typename ds::map<void *, record>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit memory(test_resource * res) noexcept;

		~memory() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static memory * const get() noexcept { return s_instance; }

		ML_NODISCARD auto allocator() const & noexcept -> allocator_type const & { return m_allocator; }

		ML_NODISCARD auto counter() const noexcept -> size_t { return m_counter; }

		ML_NODISCARD auto records() const & noexcept -> record_table const & { return m_records; }
		
		ML_NODISCARD auto resource() const noexcept -> test_resource * const { return m_resource; }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// malloc
		ML_NODISCARD void * mallocate(size_t size) noexcept
		{
			// allocate the requested bytes
			byte_t * const addr{ this->do_allocate(size) };

			// create the record
			return m_records.insert(addr, { addr, ++m_counter, size }).second->addr;
		}

		// calloc
		ML_NODISCARD void * callocate(size_t count, size_t size) noexcept
		{
			// allocate (count * size) bytes
			return this->mallocate(count * size);
		}

		// free
		void deallocate(void * addr) noexcept
		{
			// find the record
			if (auto const it{ m_records.find(addr) })
			{
				// free the allocation
				this->do_deallocate(it->second->addr, it->second->size);

				// erase the record
				m_records.erase(it->first);
			}
		}

		// realloc (sized)
		void * reallocate(void * addr, size_t oldsz, size_t newsz) noexcept
		{
			if (newsz == 0)
			{
				this->deallocate(addr);

				return nullptr;
			}
			else if (addr == nullptr)
			{
				return this->mallocate(newsz);
			}
			else if (newsz <= oldsz)
			{
				return addr;
			}
			else
			{
				void * const temp{ this->mallocate(newsz) };
				if (temp)
				{
					std::memcpy(temp, addr, oldsz);

					this->deallocate(addr);
				}
				return temp;
			}
		}

		// realloc
		void * reallocate(void * addr, size_t size) noexcept
		{
			return this->reallocate(addr, size, size);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD T * allocate_object(size_t count = 1) noexcept
		{
			return (T *)this->callocate(count, sizeof(T));
		}

		template <class T
		> void deallocate_object(T * ptr) noexcept
		{
			this->deallocate((byte_t *)ptr);
		}

		template <class T, class ... Args
		> ML_NODISCARD T * new_object(Args && ... args) noexcept
		{
			return util::construct(this->allocate_object<T>(), ML_forward(args)...);
		}

		template <class T
		> void delete_object(T * ptr) noexcept
		{
			util::destruct(ptr);

			this->deallocate_object(ptr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		ML_NODISCARD byte_t * do_allocate(size_t count) noexcept
		{
			return m_allocator.allocate(count);
		}

		void do_deallocate(byte_t * addr, size_t count) noexcept
		{
			m_allocator.deallocate(addr, count);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static memory *			s_instance	; // singleton
		allocator_type			m_allocator	; // allocator
		size_t					m_counter	; // counter
		ds::map<void *, record>	m_records	; // records
		test_resource *			m_resource	; // resource

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	static void to_json(json & j, memory::record const & v)
	{
		j["addr"	] = (intptr_t)v.addr;
		j["index"	] = v.index;
		j["size"	] = v.size;
	}

	static void from_json(json const & j, memory::record & v)
	{
		j["addr"	].get_to(*(intptr_t *)v.addr);
		j["index"	].get_to(v.index);
		j["size"	].get_to(v.size);
	}
}

namespace ml
{
	inline void * ml_malloc(size_t size) noexcept
	{
		return memory::get()->mallocate(size);
	}

	inline void * ml_calloc(size_t count, size_t size) noexcept
	{
		return memory::get()->callocate(count, size);
	}

	inline void ml_free(void * addr) noexcept
	{
		memory::get()->deallocate(addr);
	}

	inline void * ml_realloc(void * addr, size_t oldsz, size_t newsz)
	{
		return memory::get()->reallocate(addr, oldsz, newsz);
	}

	inline void * ml_realloc(void * addr, size_t size)
	{
		return memory::get()->reallocate(addr, size);
	}
}

namespace ml
{
	// trackable base
	struct ML_CORE_API trackable
	{
	public:
		virtual ~trackable() noexcept = default;

		ML_NODISCARD void * operator new(size_t size) noexcept { return ml_malloc(size); }

		ML_NODISCARD void * operator new[](size_t size) noexcept { return ml_malloc(size); }

		void operator delete(void * addr) noexcept { ml_free(addr); }

		void operator delete[](void * addr) noexcept { ml_free(addr); }
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
	> ML_NODISCARD scoped<T, Dx> alloc_scoped(Args && ... args) noexcept
	{
		return { memory::get()->new_object<T>(ML_forward(args)...), Dx{} };
	}

	// allocate shared
	template <class T, class ... Args
	> ML_NODISCARD auto alloc_shared(Args && ... args) noexcept
	{
		return std::allocate_shared<T>(memory::get()->allocator(), ML_forward(args)...);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_HPP_