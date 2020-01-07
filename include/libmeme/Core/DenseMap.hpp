#ifndef _ML_DENSE_MAP_HPP_
#define _ML_DENSE_MAP_HPP_

#include <libmeme/Core/Dense.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Basic Map Traits
	template <template <class ...> class Derived,
		class Key, class Value, class Comp, class Alloc, bool Multi
	> struct basic_map_traits : public _ML_DENSE basic_storage_traits<
		std::pair<Key, Value>, Alloc
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using derived_type = typename Derived<Key, Value, Comp, Alloc>;

		friend derived_type;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename _ML_DENSE basic_storage_traits<std::pair<Key, Value>, Alloc>;

		using key_type = typename Key;

		using mapped_type = typename Value;

		using compare_type = typename Comp;

		using search_type = typename Key const &;

		static constexpr bool multi{ Multi };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
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

	private:
		struct compare_impl final
		{
			constexpr compare_impl() noexcept = default;

			constexpr bool operator()(search_type lhs, const_reference rhs) const noexcept
			{
				return compare_type{}(lhs, rhs.first);
			}

			constexpr bool operator()(const_reference lhs, search_type rhs) const noexcept
			{
				return compare_type{}(lhs.first, rhs);
			}

			constexpr bool operator()(const_reference lhs, const_reference rhs) const noexcept
			{
				return compare_type{}(lhs.first, rhs.first);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct sort_impl final
		{
			explicit sort_impl(derived_type & value) noexcept { (*this)(value); }

			constexpr sort_impl() noexcept = default;

			inline void operator()(derived_type & value) const noexcept
			{
				std::sort(value.begin(), value.end(), compare_impl{});

				if constexpr (!multi)
				{
					value.erase(
						std::unique(value.begin(), value.end()), value.end(), compare_impl{}
					);
				}
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Basic Ordered Map
	template <
		class Key,
		class Value,
		class Comp = std::less<Value>,
		class Alloc = std::allocator<Value>
	> struct basic_ordered_map : public _ML_DENSE basic_storage<
		basic_map_traits<basic_ordered_map, Key, Value, Comp, Alloc, false>
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
		using traits_type = typename basic_map_traits<
			basic_ordered_map, Key, Value, Comp, Alloc, false
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type					= typename traits_type::derived_type;
		using key_type					= typename traits_type::key_type;
		using mapped_type				= typename traits_type::mapped_type;
		using compare_impl				= typename traits_type::compare_impl;
		using search_type				= typename traits_type::search_type;
		using base_type					= typename _ML_DENSE basic_storage<traits_type>;
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
	
		basic_ordered_map() noexcept
			: base_type{ m_storage }, m_storage{}
		{
		}

		basic_ordered_map(initializer_type init)
			: base_type{ m_storage }, m_storage{ init }
		{
			traits_type::sort_impl{ *this };
		}

		template <class It> basic_ordered_map(It first, It last)
			: base_type{ m_storage }, m_storage{ first, last }
		{
			traits_type::sort_impl{ *this };
		}

		explicit basic_ordered_map(allocator_type const & alloc)
			: base_type{ m_storage }, m_storage{ alloc }
		{
			traits_type::sort_impl{ *this };
		}
	
		explicit basic_ordered_map(storage_type const & value, allocator_type const & alloc)
			: base_type{ m_storage }, m_storage{ value, alloc }
		{
			traits_type::sort_impl{ *this };
		}
	
		explicit basic_ordered_map(storage_type && value, allocator_type const & alloc) noexcept
			: base_type{ m_storage }, m_storage{ std::move(value), alloc }
		{
			traits_type::sort_impl{ *this };
		}
	
		basic_ordered_map(self_type const & other, allocator_type const & alloc = {})
			: base_type{ m_storage }, m_storage{ other.m_storage, alloc }
		{
		}
	
		basic_ordered_map(self_type && other) noexcept
			: base_type{ m_storage }, m_storage{}
		{
			swap(std::move(other));
		}

		~basic_ordered_map() noexcept {}

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

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator lower_bound(search_type value)
		{
			return std::lower_bound(begin(), end(), value, compare_impl{});
		}

		inline const_iterator lower_bound(search_type value) const
		{
			return std::lower_bound(cbegin(), cend(), value, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator upper_bound(search_type value)
		{
			return std::upper_bound(begin(), end(), value, compare_impl{});
		}

		inline const_iterator upper_bound(search_type value) const
		{
			return std::upper_bound(cbegin(), cend(), value, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::pair<iterator, iterator> equal_range(search_type value)
		{
			return std::equal_range(begin(), end(), value, compare_impl{});
		}

		inline std::pair<const_iterator, const_iterator> equal_range(search_type value) const
		{
			return std::equal_range(cbegin(), cend(), value, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator find(search_type value)
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

		inline const_iterator find(search_type value) const
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

		inline mapped_type & at(search_type key)
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

		inline mapped_type & operator[](search_type key)
		{
			return at(key);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Basic Ordered Multimap
	template <
		class Key,
		class Value,
		class Comp = std::less<Value>,
		class Alloc = std::allocator<Value>
	> struct basic_ordered_multimap : public _ML_DENSE basic_storage<
		basic_map_traits<basic_ordered_multimap, Key, Value, Comp, Alloc, true>
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
		using traits_type = typename basic_map_traits<
			basic_ordered_multimap, Key, Value, Comp, Alloc, true
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type					= typename traits_type::derived_type;
		using key_type					= typename traits_type::key_type;
		using mapped_type				= typename traits_type::mapped_type;
		using compare_impl				= typename traits_type::compare_impl;
		using search_type				= typename traits_type::search_type;
		using base_type					= typename _ML_DENSE basic_storage<traits_type>;
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
	
		basic_ordered_multimap() noexcept
			: base_type{ m_storage }, m_storage{}
		{
		}

		basic_ordered_multimap(initializer_type init)
			: base_type{ m_storage }, m_storage{ init }
		{
			traits_type::sort_impl{ *this };
		}

		template <class It> basic_ordered_multimap(It first, It last)
			: base_type{ m_storage }, m_storage{ first, last }
		{
			traits_type::sort_impl{ *this };
		}

		explicit basic_ordered_multimap(allocator_type const & alloc)
			: base_type{ m_storage }, m_storage{ alloc }
		{
			traits_type::sort_impl{ *this };
		}
	
		explicit basic_ordered_multimap(storage_type const & value, allocator_type const & alloc)
			: base_type{ m_storage }, m_storage{ value, alloc }
		{
			traits_type::sort_impl{ *this };
		}
	
		explicit basic_ordered_multimap(storage_type && value, allocator_type const & alloc) noexcept
			: base_type{ m_storage }, m_storage{ std::move(value), alloc }
		{
			traits_type::sort_impl{ *this };
		}
	
		basic_ordered_multimap(self_type const & other, allocator_type const & alloc = {})
			: base_type{ m_storage }, m_storage{ other.m_storage, alloc }
		{
		}
	
		basic_ordered_multimap(self_type && other) noexcept
			: base_type{ m_storage }, m_storage{}
		{
			swap(std::move(other));
		}

		~basic_ordered_multimap() noexcept {}

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

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator lower_bound(search_type value)
		{
			return std::lower_bound(begin(), end(), value, compare_impl{});
		}

		inline const_iterator lower_bound(search_type value) const
		{
			return std::lower_bound(cbegin(), cend(), value, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator upper_bound(search_type value)
		{
			return std::upper_bound(begin(), end(), value, compare_impl{});
		}

		inline const_iterator upper_bound(search_type value) const
		{
			return std::upper_bound(cbegin(), cend(), value, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::pair<iterator, iterator> equal_range(search_type value)
		{
			return std::equal_range(begin(), end(), value, compare_impl{});
		}

		inline std::pair<const_iterator, const_iterator> equal_range(search_type value) const
		{
			return std::equal_range(cbegin(), cend(), value, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator find(search_type value)
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

		inline const_iterator find(search_type value) const
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
			return m_storage.emplace(equal_range(value.first).second, value);
		}

		inline iterator insert(value_type && value)
		{
			return m_storage.emplace(equal_range(value.first).second, std::move(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Ordered Map
	template <class Key, class Value
	> ML_USING ordered_map = typename basic_ordered_map<
		Key,
		Value,
		std::less<Key>,
		std::allocator<std::pair<Key, Value>>
	>;

	// Ordered Multimap
	template <class Key, class Value
	> ML_USING ordered_multimap = typename basic_ordered_multimap<
		Key,
		Value,
		std::less<Key>,
		std::allocator<std::pair<Key, Value>>
	>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DENSE_MAP_HPP_