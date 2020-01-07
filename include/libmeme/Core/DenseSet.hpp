#ifndef _ML_DENSE_SET_HPP_
#define _ML_DENSE_SET_HPP_

#include <libmeme/Core/Dense.hpp>

// Traits
namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class...> class Derived,
		class Elem, class Comp, class Alloc, bool Multi
	> struct set_traits : public _ML_DENSE storage_traits<Elem, Alloc>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static_assert(std::is_same_v<Elem, Alloc::value_type>);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type					= typename _ML_DENSE storage_traits<Elem, Alloc>;
		using self_type					= typename Derived<Elem, Comp, Alloc>;
		using compare_type				= typename Comp;
		using storage_type				= typename base_type::storage_type;
		using initializer_type			= typename base_type::initializer_type;
		using allocator_type			= typename base_type::allocator_type;
		using value_type				= typename base_type::value_type;
		using pointer					= typename base_type::pointer;
		using reference					= typename base_type::reference;
		using const_pointer				= typename base_type::const_pointer;
		using const_reference			= typename base_type::const_reference;
		using difference_type			= typename base_type::difference_type;
		using size_type					= typename base_type::size_type;
		using iterator					= typename base_type::iterator;
		using const_iterator			= typename base_type::const_iterator;
		using reverse_iterator			= typename base_type::reverse_iterator;
		using const_reverse_iterator	= typename base_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr bool multi{ Multi };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct compare_impl
		{
			constexpr compare_impl() noexcept = default;

			constexpr bool operator()(const_reference lhs, const_reference rhs) const noexcept
			{
				return compare_type{}(lhs, rhs);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Basic Set
namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Elem,
		class Comp = std::less<Elem>,
		class Alloc = std::allocator<Elem>
	> struct basic_set
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using traits_type = typename _ML_DENSE set_traits<
			basic_set, Elem, Comp, Alloc, false
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type					= typename traits_type::self_type;
		using compare_type				= typename traits_type::compare_type;
		using compare_impl				= typename traits_type::compare_impl;
		using self_type					= typename traits_type::self_type;
		using allocator_type			= typename traits_type::allocator_type;
		using value_type				= typename traits_type::value_type;
		using storage_type				= typename traits_type::storage_type;
		using initializer_type			= typename traits_type::initializer_type;
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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_set() noexcept
			: m_storage{}
		{
		}

		basic_set(initializer_type init)
			: m_storage{ init }
		{
			sort();
			erase(std::unique(begin(), end(), compare_impl{}), end());
		}

		template <class It> basic_set(It first, It last)
			: m_storage{ first, last }
		{
			sort();
			erase(std::unique(begin(), end(), compare_impl{}), end());
		}

		basic_set(storage_type const & value, allocator_type alloc = allocator_type{})
			: m_storage{ value, alloc }
		{
			sort();
			erase(std::unique(begin(), end(), compare_impl{}), end());
		}

		basic_set(storage_type && value, allocator_type alloc = allocator_type{}) noexcept
			: m_storage{ std::move(value), alloc }
		{
			setup_internal();
		}

		basic_set(basic_set const & other)
			: m_storage{ other.m_storage }
		{
		}

		basic_set(basic_set && other) noexcept
			: m_storage{}
		{
			swap(std::move(other));
		}

		~basic_set() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline basic_set & operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		inline basic_set & operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_storage, other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator lower_bound(const_reference value)
		{
			return std::lower_bound(begin(), end(), value, compare_impl{});
		}

		inline const_iterator lower_bound(const_reference value) const
		{
			return std::lower_bound(cbegin(), cend(), value, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator upper_bound(const_reference value)
		{
			return std::upper_bound(begin(), end(), value, compare_impl{});
		}

		inline const_iterator upper_bound(const_reference value) const
		{
			return std::upper_bound(cbegin(), cend(), value, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::pair<iterator, iterator> equal_range(const_reference value)
		{
			return std::equal_range(begin(), end(), value, compare_impl{});
		}

		inline std::pair<const_iterator, const_iterator> equal_range(const_reference value) const
		{
			return std::equal_range(cbegin(), cend(), value, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator find(const_reference value)
		{
			if (auto const it{ equal_range(value) }; it.first != it.second)
			{
				return it.first;
			}
			else
			{
				return end();
			}
		}

		inline const_iterator find(const_reference value) const
		{
			if (auto const it{ equal_range(value) }; it.first != it.second)
			{
				return it.first;
			}
			else
			{
				return cend();
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::pair<iterator, bool> insert(const_reference value)
		{
			if (auto const it{ equal_range(value) }; it.first != it.second)
			{
				return std::make_pair(it.second, false);
			}
			else
			{
				return std::make_pair(m_storage.emplace(it.second, value), true);
			}
		}

		inline std::pair<iterator, bool> insert(value_type && value)
		{
			if (auto const it{ equal_range(value) }; it.first != it.second)
			{
				return std::make_pair(it.second, false);
			}
			else
			{
				return std::make_pair(m_storage.emplace(it.second, std::move(value)), true);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto back() noexcept -> reference { return m_storage.back(); }

		inline auto back() const noexcept -> const_reference { return m_storage.back(); }

		inline auto begin() noexcept -> iterator { return m_storage.begin(); }

		inline auto begin() const noexcept -> const_iterator { return m_storage.begin(); }

		inline auto capacity() const noexcept -> size_type { return m_storage.capacity(); }

		inline auto cbegin() const noexcept -> const_iterator { return m_storage.cbegin(); }

		inline auto cend() const noexcept -> const_iterator { return m_storage.cend(); }

		inline void clear() noexcept { return m_storage.clear(); }

		inline auto crbegin() const noexcept -> const_reverse_iterator { return m_storage.crbegin(); }

		inline auto crend() const noexcept -> const_reverse_iterator { return m_storage.crend(); }

		inline auto data() noexcept -> pointer { return m_storage.data(); }

		inline auto data() const noexcept -> const_pointer { return m_storage.data(); }

		inline bool empty() const noexcept { return m_storage.empty(); }

		inline auto end() noexcept -> iterator { return m_storage.end(); }

		inline auto end() const noexcept -> const_iterator { return m_storage.end(); }

		inline auto erase(iterator value) -> iterator { return m_storage.erase(value); }

		inline auto erase(iterator first, iterator last) -> iterator { return m_storage.erase(first, last); }

		inline auto front() noexcept -> reference { return m_storage.front(); }

		inline auto front() const noexcept -> const_reference { return m_storage.front(); }

		inline auto get_allocator() const noexcept -> allocator_type { return m_storage.get_allocator(); }

		inline auto max_size() const noexcept -> size_type { return m_storage.max_size(); }

		inline auto rbegin() noexcept -> reverse_iterator { return m_storage.rbegin(); }

		inline auto rbegin() const noexcept -> const_reverse_iterator { return m_storage.rbegin(); }

		inline auto rend() noexcept -> reverse_iterator { return m_storage.rend(); }

		inline auto rend() const noexcept -> const_reverse_iterator { return m_storage.rend(); }

		inline void reserve(size_type const value) { return m_storage.reserve(value); }

		inline void shrink_to_fit() { return m_storage.shrink_to_fit(); }

		inline auto size() const noexcept -> size_type { return m_storage.size(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		inline void setup_internal()
		{
			std::sort(begin(), end(), compare_impl{});

			erase(std::unique(begin(), end()), end(), compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Basic Multiset
namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Elem,
		class Comp = std::less<Elem>,
		class Alloc = std::allocator<Elem>
	> struct basic_multiset
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using traits_type = typename _ML_DENSE set_traits<
			basic_multiset, Elem, Comp, Alloc, true
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type					= typename traits_type::self_type;
		using compare_type				= typename traits_type::compare_type;
		using compare_impl				= typename traits_type::compare_impl;
		using self_type					= typename traits_type::self_type;
		using allocator_type			= typename traits_type::allocator_type;
		using value_type				= typename traits_type::value_type;
		using storage_type				= typename traits_type::storage_type;
		using initializer_type			= typename traits_type::initializer_type;
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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_multiset() noexcept
			: m_storage{}
		{
		}

		basic_multiset(initializer_type init)
			: m_storage{ init }
		{
			setup_internal();
		}

		template <class It> basic_multiset(It first, It last)
			: m_storage{ first, last }
		{
			setup_internal();
		}

		basic_multiset(storage_type const & value, allocator_type alloc = allocator_type{})
			: m_storage{ value, alloc }
		{
			setup_internal();
		}

		basic_multiset(storage_type && value, allocator_type alloc = allocator_type{}) noexcept
			: m_storage{ std::move(value), alloc }
		{
			setup_internal();
		}

		basic_multiset(basic_multiset const & other)
			: m_storage{ other.m_storage }
		{
		}

		basic_multiset(basic_multiset && other) noexcept
			: m_storage{}
		{
			swap(std::move(other));
		}

		~basic_multiset() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline basic_multiset & operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		inline basic_multiset & operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_storage, other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator lower_bound(const_reference value)
		{
			return std::lower_bound(begin(), end(), value, compare_impl{});
		}

		inline const_iterator lower_bound(const_reference value) const
		{
			return std::lower_bound(cbegin(), cend(), value, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator upper_bound(const_reference value)
		{
			return std::upper_bound(begin(), end(), value, compare_impl{});
		}

		inline const_iterator upper_bound(const_reference value) const
		{
			return std::upper_bound(cbegin(), cend(), value, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::pair<iterator, iterator> equal_range(const_reference value)
		{
			return std::equal_range(begin(), end(), value, compare_impl{});
		}

		inline std::pair<const_iterator, const_iterator> equal_range(const_reference value) const
		{
			return std::equal_range(cbegin(), cend(), value, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator find(const_reference value)
		{
			if (auto const it{ equal_range(value) }; it.first != it.second)
			{
				return it.first;
			}
			else
			{
				return end();
			}
		}

		inline const_iterator find(const_reference value) const
		{
			if (auto const it{ equal_range(value) }; it.first != it.second)
			{
				return it.first;
			}
			else
			{
				return cend();
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator insert(const_reference value)
		{
			return m_storage.emplace(upper_bound(value), value);
		}

		inline iterator insert(value_type && value)
		{
			return m_storage.emplace(upper_bound(value), std::move(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto back() noexcept -> reference { return m_storage.back(); }

		inline auto back() const noexcept -> const_reference { return m_storage.back(); }

		inline auto begin() noexcept -> iterator { return m_storage.begin(); }

		inline auto begin() const noexcept -> const_iterator { return m_storage.begin(); }

		inline auto capacity() const noexcept -> size_type { return m_storage.capacity(); }

		inline auto cbegin() const noexcept -> const_iterator { return m_storage.cbegin(); }

		inline auto cend() const noexcept -> const_iterator { return m_storage.cend(); }

		inline void clear() noexcept { return m_storage.clear(); }

		inline auto crbegin() const noexcept -> const_reverse_iterator { return m_storage.crbegin(); }

		inline auto crend() const noexcept -> const_reverse_iterator { return m_storage.crend(); }

		inline auto data() noexcept -> pointer { return m_storage.data(); }

		inline auto data() const noexcept -> const_pointer { return m_storage.data(); }

		inline bool empty() const noexcept { return m_storage.empty(); }

		inline auto end() noexcept -> iterator { return m_storage.end(); }

		inline auto end() const noexcept -> const_iterator { return m_storage.end(); }

		inline auto erase(iterator value) -> iterator { return m_storage.erase(value); }

		inline auto erase(iterator first, iterator last) -> iterator { return m_storage.erase(first, last); }

		inline auto front() noexcept -> reference { return m_storage.front(); }

		inline auto front() const noexcept -> const_reference { return m_storage.front(); }

		inline auto get_allocator() const noexcept -> allocator_type { return m_storage.get_allocator(); }

		inline auto max_size() const noexcept -> size_type { return m_storage.max_size(); }

		inline auto rbegin() noexcept -> reverse_iterator { return m_storage.rbegin(); }

		inline auto rbegin() const noexcept -> const_reverse_iterator { return m_storage.rbegin(); }

		inline auto rend() noexcept -> reverse_iterator { return m_storage.rend(); }

		inline auto rend() const noexcept -> const_reverse_iterator { return m_storage.rend(); }

		inline void reserve(size_type const value) { return m_storage.reserve(value); }

		inline void shrink_to_fit() { return m_storage.shrink_to_fit(); }

		inline auto size() const noexcept -> size_type { return m_storage.size(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		inline void setup_internal()
		{
			std::sort(begin(), end(), compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Typedefs
namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Set
	template <class Elem
	> ML_USING set = typename _ML_DENSE basic_set<
		Elem,
		std::less<Elem>,
		std::allocator<Elem>
	>;

	// Multiset
	template <class Elem
	> ML_USING multiset = typename _ML_DENSE basic_multiset<
		Elem,
		std::less<Elem>,
		std::allocator<Elem>
	>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DENSE_SET_HPP_