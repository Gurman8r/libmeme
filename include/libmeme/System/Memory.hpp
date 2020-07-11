#ifndef _ML_MEMORY_HPP_
#define _ML_MEMORY_HPP_

#include <libmeme/System/Export.hpp>
#include <libmeme/System/Debug.hpp>
#include <libmeme/Core/FlatMap.hpp>
#include <libmeme/Core/Singleton.hpp>

namespace ml::util
{
	// passthrough for collecting metrics on upstream resource
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
	// memory manager singleton
	class ML_SYSTEM_API memory final : public singleton<memory>
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// allocator type
		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		// memory record
		struct ML_NODISCARD record final
		{
			size_t index; size_t size; byte_t * data;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static auto const & get_allocator() noexcept { return get_instance().m_allocator; }

		ML_NODISCARD static auto const & get_records() noexcept { return get_instance().m_records; }

		ML_NODISCARD static auto const & get_test_resource() noexcept { return get_instance().m_testres; }

		ML_NODISCARD static auto const & set_test_resource(util::test_resource * res) noexcept { return get_instance().m_testres = res; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		// malloc
		ML_NODISCARD static void * allocate(size_t size) noexcept
		{
			static auto & self{ get_instance() };

			// allocate the requested bytes
			byte_t * const data{ self.m_allocator.allocate(size) };

			// create the record
			return self.m_records.insert(data, { self.m_index++, size, data }).second->data;
		}

		// calloc
		ML_NODISCARD static void * allocate(size_t count, size_t size) noexcept
		{
			// allocate (count * size) zeroed bytes
			return std::memset(allocate(count * size), 0, count * size);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// free
		static void deallocate(void * addr) noexcept
		{
			static auto & self{ get_instance() };

			// find the record
			if (auto const it{ self.m_records.find(addr) })
			{
				// free the allocation
				self.m_allocator.deallocate(it->second->data, it->second->size);

				// erase the record
				self.m_records.erase(it->first);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// realloc
		ML_NODISCARD static void * reallocate(void * addr, size_t size) noexcept
		{
			return reallocate(addr, size, size);
		}

		// realloc (sized)
		ML_NODISCARD static void * reallocate(void * addr, size_t oldsz, size_t newsz) noexcept
		{
			if (newsz == 0)
			{
				deallocate(addr);

				return nullptr;
			}
			else if (addr == nullptr)
			{
				return allocate(newsz);
			}
			else if (newsz <= oldsz)
			{
				return addr;
			}
			else
			{
				void * const temp{ allocate(newsz) };
				if (temp)
				{
					std::memcpy(temp, addr, oldsz);

					deallocate(addr);
				}
				return temp;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend singleton;

		~memory() noexcept;

		allocator_type			m_allocator	{};	// allocator
		size_t					m_index		{};	// record index
		ds::map<void *, record>	m_records	{};	// memory records
		util::test_resource *	m_testres	{};	// pointer to test resource

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

namespace ml
{
	// trackable base
	struct ML_SYSTEM_API trackable
	{
		virtual ~trackable() noexcept = default;

		ML_NODISCARD void * operator new(size_t size) noexcept { return memory::allocate(size); }

		ML_NODISCARD void * operator new[](size_t size) noexcept { return memory::allocate(size); }

		void operator delete(void * addr) noexcept { memory::deallocate(addr); }

		void operator delete[](void * addr) noexcept { memory::deallocate(addr); }
	};
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ...
	> struct default_delete;

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
			memory::deallocate(addr);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// shared pointer
	template <class T
	> ML_alias shared = typename std::shared_ptr<T>;

	// unique pointer
	template <class T, class Dx = default_delete<T>
	> ML_alias unique = typename std::unique_ptr<T, Dx>;

	// weak pointer
	template <class T
	> ML_alias weak = typename std::weak_ptr<T>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// make new
	template <class T, class ... Args
	> ML_NODISCARD T * make_new(Args && ... args) noexcept
	{
		return ::new (memory::allocate(sizeof(T))) T{ ML_forward(args)... };
	}

	// make shared
	template <class T, class ... Args
	> ML_NODISCARD shared<T> make_shared(Args && ... args) noexcept
	{
		return std::allocate_shared<T>(memory::get_allocator(), ML_forward(args)...);
	}

	// make unique
	template <class T, class Dx = default_delete<T>, class ... Args
	> ML_NODISCARD unique<T, Dx> make_unique(Args && ... args) noexcept
	{
		return unique<T, Dx>{ _ML make_new<T>(ML_forward(args)...), Dx{} };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MEMORY_HPP_