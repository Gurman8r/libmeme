#ifndef _ML_DENSE_MAP_HPP_
#define _ML_DENSE_MAP_HPP_

#include <libmeme/Core/Dense.hpp>

// Traits
namespace ml::dense::detail
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class...> class Derived,
		class Key, class Value, class Comp, class Alloc, bool Multi
	> struct map_traits : public storage_traits<std::pair<Key, Value>, Alloc>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static_assert(std::is_same_v<std::pair<Key, Value>, Alloc::value_type>);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type					= typename storage_traits<std::pair<Key, Value>, Alloc>;
		using self_type					= typename Derived<Key, Value, Comp, Alloc>;
		using key_type					= typename Key;
		using mapped_type				= typename Value;
		using compare_type				= typename Comp;
		using allocator_type			= typename base_type::allocator_type;
		using storage_type				= typename base_type::storage_type;
		using initializer_type			= typename base_type::initializer_type;
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

			constexpr bool operator()(const_reference lhs, key_type const & rhs) const noexcept
			{
				return compare_type{}(lhs.first, rhs);
			}

			constexpr bool operator()(key_type const & lhs, const_reference rhs) const noexcept
			{
				return compare_type{}(lhs, rhs.first);
			}

			constexpr bool operator()(const_reference lhs, const_reference rhs) const noexcept
			{
				return compare_type{}(lhs.first, rhs.first);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Base
namespace ml::dense::detail
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Traits
	> struct map_base : public storage_base<Traits>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using traits_type				= typename Traits;
		using self_type					= typename traits_type::self_type;
		using key_type					= typename traits_type::key_type;
		using mapped_type				= typename traits_type::mapped_type;
		using allocator_type			= typename traits_type::allocator_type;
		using compare_type				= typename traits_type::compare_type;
		using compare_impl				= typename traits_type::compare_impl;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type					= typename storage_base<traits_type>;
		using storage_type				= typename base_type::storage_type;
		using initializer_type			= typename base_type::initializer_type;
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
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Basic Map
namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Key, class Value,
		class Comp = std::less<Key>,
		class Alloc = std::allocator<std::pair<Key, Value>>
	> struct basic_map : public detail::map_base<
		detail::map_traits<basic_map, Key, Value, Comp, Alloc, false>
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename detail::map_base<
			detail::map_traits<basic_map, Key, Value, Comp, Alloc, false>
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using key_type					= typename base_type::key_type;
		using mapped_type				= typename base_type::mapped_type;
		using compare_type				= typename base_type::compare_type;
		using allocator_type			= typename base_type::allocator_type;
		using value_type				= typename base_type::value_type;
		using compare_impl				= typename base_type::compare_impl;
		using self_type					= typename base_type::self_type;
		using storage_type				= typename base_type::storage_type;
		using initializer_type			= typename base_type::initializer_type;
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

		basic_map() noexcept
			: m_storage{}
		{
		}

		basic_map(initializer_type init)
			: m_storage{ init }
		{
			sort();
			cull();
		}

		template <class It> basic_map(It first, It last)
			: m_storage{ first, last }
		{
			sort();
			cull();
		}

		basic_map(storage_type const & value, allocator_type const & alloc = allocator_type{})
			: m_storage{ value, alloc }
		{
			sort();
			cull();
		}

		basic_map(storage_type && value, allocator_type const & alloc = allocator_type{}) noexcept
			: m_storage{ std::move(value), alloc }
		{
			sort();
			cull();
		}

		basic_map(self_type const & value)
			: m_storage{ value.m_storage }
		{
		}

		basic_map(self_type && value) noexcept
			: m_storage{}
		{
			swap(std::move(value));
		}

		~basic_map() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(self_type const & value)
		{
			self_type temp{ value };
			swap(temp);
			return (*this);
		}

		inline self_type & operator=(self_type && value) noexcept
		{
			swap(std::move(value));
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator lower_bound(key_type const & key)
		{
			return std::lower_bound(begin(), end(), key, compare_impl{});
		}

		inline const_iterator lower_bound(key_type const & key) const
		{
			return std::lower_bound(cbegin(), cend(), key, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator upper_bound(key_type const & key)
		{
			return std::upper_bound(begin(), end(), key, compare_impl{});
		}

		inline const_iterator upper_bound(key_type const & key) const
		{
			return std::upper_bound(cbegin(), cend(), key, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::pair<iterator, iterator> equal_range(key_type const & key)
		{
			return std::equal_range(begin(), end(), key, compare_impl{});
		}

		inline std::pair<const_iterator, const_iterator> equal_range(key_type const & key) const
		{
			return std::equal_range(cbegin(), cend(), key, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator find(key_type const & key)
		{
			if (auto const it{ equal_range(key) }; it.first != it.second)
			{
				return it.first;
			}
			else
			{
				return end();
			}
		}

		inline const_iterator find(key_type const & key) const
		{
			if (auto const it{ equal_range(key) }; it.first != it.second)
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
			if (auto const it{ equal_range(value.first) }; it.first != it.second)
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
			if (auto const it{ equal_range(value.first) }; it.first != it.second)
			{
				return std::make_pair(it.second, false);
			}
			else
			{
				return std::make_pair(m_storage.emplace(it.second, std::move(value)), true);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline mapped_type & at(key_type const & key)
		{
			if (auto const it{ equal_range(key) }; it.first != it.second)
			{
				return it.first->second;
			}
			else
			{
				return m_storage.emplace(it.second, value_type{ key, mapped_type{} })->second;
			}
		}

		inline mapped_type & operator[](key_type const & key)
		{
			return at(key);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void clear() noexcept
		{
			m_storage.clear();
		}

		inline void cull()
		{
			erase(std::unique(begin(), end(), compare_impl{}), end());
		}

		inline iterator erase(const_iterator it)
		{
			return m_storage.erase(it);
		}

		inline iterator erase(const_iterator first, const_iterator last)
		{
			return m_storage.erase(first, last);
		}

		inline void reserve(size_type const size)
		{
			m_storage.reserve(size);
		}

		inline void shrink_to_fit()
		{
			m_storage.shrink_to_fit();
		}

		inline void sort() noexcept
		{
			std::sort(begin(), end(), compare_impl{});
		}

		inline void swap(self_type & value) noexcept
		{
			if (this != std::addressof(value))
			{
				std::swap(m_storage, value.m_storage);
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

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Basic Multimap
namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Key, class Value,
		class Comp = std::less<Key>,
		class Alloc = std::allocator<std::pair<Key, Value>>
	> struct basic_multimap : public detail::map_base<
		detail::map_traits<basic_map, Key, Value, Comp, Alloc, true>
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename detail::map_base<
			detail::map_traits<basic_map, Key, Value, Comp, Alloc, true>
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using key_type					= typename base_type::key_type;
		using mapped_type				= typename base_type::mapped_type;
		using compare_type				= typename base_type::compare_type;
		using value_type				= typename base_type::value_type;
		using allocator_type			= typename base_type::allocator_type;
		using compare_impl				= typename base_type::compare_impl;
		using self_type					= typename base_type::self_type;
		using storage_type				= typename base_type::storage_type;
		using initializer_type			= typename base_type::initializer_type;
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

		basic_multimap() noexcept
			: m_storage{}
		{
		}

		basic_multimap(initializer_type init)
			: m_storage{ init }
		{
			sort();
			cull();
		}

		template <class It> basic_multimap(It first, It last)
			: m_storage{ first, last }
		{
			sort();
			cull();
		}

		basic_multimap(storage_type const & value, allocator_type alloc = allocator_type{})
			: m_storage{ value, alloc }
		{
			sort();
			cull();
		}

		basic_multimap(storage_type && value, allocator_type alloc = allocator_type{}) noexcept
			: m_storage{ std::move(value), alloc }
		{
			sort();
			cull();
		}

		basic_multimap(self_type const & other)
			: m_storage{}
		{
		}

		basic_multimap(self_type && other) noexcept
			: m_storage{}
		{
			swap(std::move(other));
		}

		~basic_multimap() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline basic_multimap & operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		inline basic_multimap & operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator lower_bound(key_type const & key)
		{
			return std::lower_bound(begin(), end(), key, compare_impl{});
		}

		inline const_iterator lower_bound(key_type const & key) const
		{
			return std::lower_bound(cbegin(), cend(), key, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator upper_bound(key_type const & key)
		{
			return std::upper_bound(begin(), end(), key, compare_impl{});
		}

		inline const_iterator upper_bound(key_type const & key) const
		{
			return std::upper_bound(cbegin(), cend(), key, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::pair<iterator, iterator> equal_range(key_type const & key)
		{
			return std::equal_range(begin(), end(), key, compare_impl{});
		}

		inline std::pair<const_iterator, const_iterator> equal_range(key_type const & key) const
		{
			return std::equal_range(cbegin(), cend(), key, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator find(key_type const & key)
		{
			if (auto const it{ equal_range(key) }; it.first != it.second)
			{
				return it.first;
			}
			else
			{
				return end();
			}
		}

		inline const_iterator find(key_type const & key) const
		{
			if (auto const it{ equal_range(key) }; it.first != it.second)
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
			return m_storage.emplace(upper_bound(value.first), value);
		}

		inline iterator insert(value_type && value)
		{
			return m_storage.emplace(upper_bound(value.first), std::move(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void clear() noexcept
		{
			m_storage.clear();
		}

		inline void cull()
		{
			erase(std::unique(begin(), end(), compare_impl{}), end());
		}

		inline iterator erase(iterator value)
		{
			return m_storage.erase(value);
		}

		inline iterator erase(iterator first, iterator last)
		{
			return m_storage.erase(first, last);
		}

		inline void reserve(size_type const value)
		{
			m_storage.reserve(value);
		}

		inline void shrink_to_fit()
		{
			m_storage.shrink_to_fit();
		}

		inline void sort() noexcept
		{
			std::sort(begin(), end(), compare_impl{});
		}

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_storage, other.m_storage);
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

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Typedefs
namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Map
	template <class Key, class Value
	> ML_USING map = typename _ML_DENSE basic_map<
		Key, Value,
		std::less<Key>,
		std::allocator<std::pair<Key, Value>>
	>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Multimap
	template <class Key, class Value
	> ML_USING multimap = typename _ML_DENSE basic_multimap<
		Key, Value,
		std::less<Key>,
		std::allocator<std::pair<Key, Value>>
	>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DENSE_MAP_HPP_