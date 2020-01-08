#ifndef _ML_DENSE_HPP_
#define _ML_DENSE_HPP_

#include <libmeme/Core/Core.hpp>

#define _ML_DENSE _ML dense::

namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Basic Dense Container Traits
	template <class Elem, class Alloc, class Comp
	> struct basic_container_traits
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type				= typename Elem;
		using allocator_type			= typename Alloc;
		using compare_type				= typename Comp;
		using storage_type				= typename std::vector<value_type, allocator_type>;
		using pointer					= typename storage_type::pointer;
		using reference					= typename storage_type::reference;
		using const_pointer				= typename storage_type::const_pointer;
		using const_reference			= typename storage_type::const_reference;
		using difference_type			= typename storage_type::difference_type;
		using size_type					= typename storage_type::size_type;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;
		using initializer_type			= typename std::initializer_list<value_type>;
		using iterator_pair				= typename std::pair<iterator, iterator>;
		using const_iterator_pair		= typename std::pair<const_iterator, const_iterator>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// Basic Dense Container
	template <class Traits
	> struct basic_container
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using traits_type				= typename Traits;
		using self_type					= typename basic_container<traits_type>;
		using value_type				= typename traits_type::value_type;
		using compare_type				= typename traits_type::compare_type;
		using allocator_type			= typename traits_type::allocator_type;
		using storage_type				= typename traits_type::storage_type;
		using pointer					= typename traits_type::pointer;
		using reference					= typename traits_type::reference;
		using const_pointer				= typename traits_type::const_pointer;
		using const_reference			= typename traits_type::const_reference;
		using difference_type			= typename traits_type::difference_type;
		using size_type					= typename traits_type::size_type;
		using iterator					= typename traits_type::iterator;
		using const_iterator			= typename traits_type::const_iterator;
		using reverse_iterator			= typename traits_type::reverse_iterator;
		using const_reverse_iterator	= typename traits_type::const_reverse_iterator;
		using initializer_type			= typename traits_type::initializer_type;
		using iterator_pair				= typename traits_type::iterator_pair;
		using const_iterator_pair		= typename traits_type::const_iterator_pair;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_container() noexcept
			: m_storage{}
		{
		}

		explicit basic_container(allocator_type const & alloc)
			: m_storage{ alloc }
		{
		}

		explicit basic_container(size_type const count, allocator_type const & alloc = {})
			: m_storage{ count, alloc }
		{
		}

		basic_container(size_type const count, const_reference value, allocator_type const & alloc = {})
			: m_storage{ count, value, alloc }
		{
		}

		template <class It> basic_container(It first, It last)
			: m_storage{ first, last }
		{
		}

		basic_container(initializer_type init)
			: m_storage{ init }
		{
		}

		explicit basic_container(storage_type const & value)
			: m_storage{ value }
		{
		}

		explicit basic_container(storage_type && value) noexcept
			: m_storage{ std::move(value) }
		{
		}

		explicit basic_container(storage_type const & value, allocator_type const & alloc)
			: m_storage{ value, alloc }
		{
		}

		explicit basic_container(storage_type && value, allocator_type const & alloc) noexcept
			: m_storage{ std::move(value), alloc }
		{
		}

		basic_container(self_type const & other)
			: m_storage{ other.m_storage }
		{
		}

		basic_container(self_type && other) noexcept
			: m_storage{ std::move(other.m_storage) }
		{
		}

		basic_container(self_type const & other, allocator_type const & alloc)
			: m_storage{ other.m_storage, alloc }
		{
		}

		basic_container(self_type && other, allocator_type const & alloc) noexcept
			: m_storage{ std::move(other.m_storage), alloc }
		{
		}

		~basic_container() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
		inline self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		inline self_type & operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline self_type & operator=(initializer_type init)
		{
			self_type temp{ init };
			swap(temp);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void clear() noexcept
		{
			return m_storage.clear();
		}

		inline void reserve(size_type const count)
		{
			return m_storage.reserve(count);
		}

		inline void resize(size_type const count)
		{
			return m_storage.resize(count);
		}

		inline void shrink_to_fit()
		{
			return m_storage.shrink_to_fit();
		}

		inline void swap(self_type & other) noexcept
		{
			return m_storage.swap(other.m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator erase(iterator location)
		{
			return m_storage.erase(location);
		}

		inline iterator erase(iterator first, iterator last)
		{
			return m_storage.erase(first, last);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void sort() noexcept
		{
			return sort(begin(), end());
		}

		inline void sort(iterator first, iterator last) noexcept
		{
			return std::sort(first, last, compare_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline bool binary_search(U && u) const
		{
			return std::binary_search(cbegin(), cend(), std::forward<U>(u), compare_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline iterator_pair equal_range(U && u)
		{
			return std::equal_range(begin(), end(), std::forward<U>(u), compare_type{});
		}

		template <class U> inline const_iterator_pair equal_range(U && u) const
		{
			return std::equal_range(cbegin(), cend(), std::forward<U>(u), compare_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline iterator lower_bound(U && u)
		{
			return std::lower_bound(begin(), end(), std::forward<U>(u), compare_type{});
		}

		template <class U> inline const_iterator lower_bound(U && u) const
		{
			return std::lower_bound(cbegin(), cend(), std::forward<U>(u), compare_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline iterator upper_bound(U && u)
		{
			return std::upper_bound(begin(), end(), std::forward<U>(u), compare_type{});
		}

		template <class U> inline const_iterator upper_bound(U && u) const
		{
			return std::upper_bound(cbegin(), cend(), std::forward<U>(u), compare_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto back() noexcept -> reference { return m_storage.back(); }

		inline auto back() const noexcept -> const_reference { return m_storage.back(); }

		inline auto begin() noexcept -> iterator { return m_storage.begin(); }

		inline auto begin() const noexcept -> const_iterator { return m_storage.begin(); }

		inline auto capacity() const noexcept -> size_type { return m_storage.capacity(); }

		inline auto cbegin() const noexcept -> const_iterator { return m_storage.cbegin(); }

		inline auto cend() const noexcept -> const_iterator { return m_storage.cend(); }

		inline auto crbegin() const noexcept -> const_reverse_iterator { return m_storage.crbegin(); }

		inline auto crend() const noexcept -> const_reverse_iterator { return m_storage.crend(); }

		inline auto data() noexcept -> pointer { return m_storage.data(); }

		inline auto data() const noexcept -> const_pointer { return m_storage.data(); }

		inline bool empty() const noexcept { return m_storage.empty(); }

		inline auto end() noexcept -> iterator { return m_storage.end(); }

		inline auto end() const noexcept -> const_iterator { return m_storage.end(); }

		inline auto front() noexcept -> reference { return m_storage.front(); }

		inline auto front() const noexcept -> const_reference { return m_storage.front(); }

		inline auto get_allocator() const noexcept -> allocator_type { return m_storage.get_allocator(); }

		inline auto max_size() const noexcept -> size_type { return m_storage.max_size(); }

		inline auto rbegin() noexcept -> reverse_iterator { return m_storage.rbegin(); }

		inline auto rbegin() const noexcept -> const_reverse_iterator { return m_storage.rbegin(); }

		inline auto rend() noexcept -> reverse_iterator { return m_storage.rend(); }

		inline auto rend() const noexcept -> const_reverse_iterator { return m_storage.rend(); }

		inline auto size() const noexcept -> size_type { return m_storage.size(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		union { storage_type m_storage; };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DENSE_HPP_