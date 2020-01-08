#ifndef _ML_DENSE_MAP_HPP_
#define _ML_DENSE_MAP_HPP_

#include <libmeme/Core/Dense.hpp>

// Basic Map Traits
namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class ...> class Derived,
		class Key, class Value, class Comp, class Alloc, bool Multi
	> struct basic_map_traits : public basic_storage_traits<std::pair<Key, Value>, Alloc>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename basic_storage_traits<std::pair<Key, Value>, Alloc>;
		
		using derived_type = typename Derived<Key, Value, Comp, Alloc>;

		using key_type = typename Key;

		using mapped_type = typename Value;

		using compare_type = typename Comp;

		static constexpr bool multi{ Multi };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using allocator_type			= typename base_type::allocator_type;
		using storage_type				= typename base_type::storage_type;
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
		using initializer_type			= typename base_type::initializer_type;
		using iterator_pair				= typename base_type::iterator_pair;
		using const_iterator_pair		= typename base_type::const_iterator_pair;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct compare_impl
		{
			constexpr compare_impl() noexcept = default;

			constexpr bool operator()(key_type const & lhs, const_reference rhs) const noexcept
			{
				return compare_type{}(lhs, rhs.first);
			}

			constexpr bool operator()(const_reference lhs, key_type const & rhs) const noexcept
			{
				return compare_type{}(lhs.first, rhs);
			}

			constexpr bool operator()(const_reference lhs, const_reference rhs) const noexcept
			{
				return compare_type{}(lhs.first, rhs.first);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct sort_impl
		{
			constexpr sort_impl() noexcept = default;

			inline void operator()(derived_type & value) const noexcept
			{
				std::sort(value.begin(), value.end());

				if constexpr (!multi)
				{
					value.erase(std::unique(value.begin(), value.end()), value.end());
				}
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Basic Ordered Map
namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Key, class Value,
		class Comp = std::less<Value>,
		class Alloc = std::allocator<Value>
	> struct basic_ordered_map : public basic_storage<basic_map_traits<
		basic_ordered_map, Key, Value, Comp, Alloc, false
	>>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
		using traits_type = basic_map_traits<basic_ordered_map,
			Key, Value, Comp, Alloc, false
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type					= typename basic_storage<traits_type>;
		using self_type					= typename traits_type::derived_type;
		using key_type					= typename traits_type::key_type;
		using mapped_type				= typename traits_type::mapped_type;
		using compare_impl				= typename traits_type::compare_impl;
		using allocator_type			= typename base_type::allocator_type;
		using storage_type				= typename base_type::storage_type;
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
		using initializer_type			= typename base_type::initializer_type;
		using iterator_pair				= typename base_type::iterator_pair;
		using const_iterator_pair		= typename base_type::const_iterator_pair;
	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
		basic_ordered_map() noexcept
			: base_type{}
		{
		}

		basic_ordered_map(initializer_type init)
			: base_type{ init }
		{
			traits_type::sort_impl{}(*this);
		}

		template <class It> basic_ordered_map(It first, It last)
			: base_type{ first, last }
		{
			traits_type::sort_impl{}(*this);
		}

		explicit basic_ordered_map(allocator_type const & alloc)
			: base_type{ alloc }
		{
			traits_type::sort_impl{}(*this);
		}
	
		explicit basic_ordered_map(storage_type const & value, allocator_type const & alloc = {})
			: base_type{ value, alloc }
		{
			traits_type::sort_impl{}(*this);
		}
	
		explicit basic_ordered_map(storage_type && value, allocator_type const & alloc = {}) noexcept
			: base_type{ std::move(value), alloc }
		{
			traits_type::sort_impl{}(*this);
		}
	
		basic_ordered_map(self_type const & other, allocator_type const & alloc = {})
			: base_type{ other, alloc }
		{
		}
	
		basic_ordered_map(self_type && other, allocator_type const & alloc = {}) noexcept
			: base_type{ std::move(other), alloc }
		{
		}

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline bool binary_search(U && u) const
		{
			return std::binary_search(cbegin(), cend(), std::forward<U>(u), compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline iterator_pair equal_range(U && u)
		{
			return std::equal_range(begin(), end(), std::forward<U>(u), compare_impl{});
		}

		template <class U> inline const_iterator_pair equal_range(U && u) const
		{
			return std::equal_range(cbegin(), cend(), std::forward<U>(u), compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline iterator lower_bound(U && u)
		{
			return std::lower_bound(begin(), end(), std::forward<U>(u), compare_impl{});
		}

		template <class U> inline const_iterator lower_bound(U && u) const
		{
			return std::lower_bound(cbegin(), cend(), std::forward<U>(u), compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline iterator upper_bound(U && u)
		{
			return std::upper_bound(begin(), end(), std::forward<U>(u), compare_impl{});
		}

		template <class U> inline const_iterator upper_bound(U && u) const
		{
			return std::upper_bound(cbegin(), cend(), std::forward<U>(u), compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline  iterator find(U && u)
		{
			if (auto const it{ equal_range(std::forward<U>(u)) }; it.first != it.second)
			{
				return it.first;
			}
			return end();
		}

		template <class U> inline  const_iterator find(U && u) const
		{
			if (auto const it{ equal_range(std::forward<U>(u)) }; it.first != it.second)
			{
				return it.first;
			}
			return cend();
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
				return std::make_pair(emplace(it.second, value), true);
			}
		}

		inline std::pair<iterator, bool> insert(value_type && value)
		{
			if (auto const it{ equal_range(std::move(value.first)) }; it.first != it.second)
			{
				return std::make_pair(it.second, false);
			}
			else
			{
				return std::make_pair(emplace(it.second, std::move(value)), true);
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
				return emplace(it.second, value_type{ key, mapped_type{} })->second;
			}
		}

		inline mapped_type & operator[](key_type const & key)
		{
			return at(key);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Basic Ordered Multimap
namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Key, class Value,
		class Comp = std::less<Value>,
		class Alloc = std::allocator<Value>
	> struct basic_ordered_multimap : public basic_storage<basic_map_traits<
		basic_ordered_multimap, Key, Value, Comp, Alloc, true
	>>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
		using traits_type = basic_map_traits<basic_ordered_multimap,
			Key, Value, Comp, Alloc, true
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type					= typename basic_storage<traits_type>;
		using self_type					= typename traits_type::derived_type;
		using key_type					= typename traits_type::key_type;
		using mapped_type				= typename traits_type::mapped_type;
		using compare_impl				= typename traits_type::compare_impl;
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
		using iterator_pair				= typename base_type::iterator_pair;
		using const_iterator_pair		= typename base_type::const_iterator_pair;
	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
		basic_ordered_multimap() noexcept
			: base_type{}
		{
		}

		basic_ordered_multimap(initializer_type init)
			: base_type{ init }
		{
			traits_type::sort_impl{}(*this);
		}

		template <class It> basic_ordered_multimap(It first, It last)
			: base_type{ first, last }
		{
			traits_type::sort_impl{}(*this);
		}

		explicit basic_ordered_multimap(allocator_type const & alloc)
			: base_type{ alloc }
		{
			traits_type::sort_impl{}(*this);
		}
	
		explicit basic_ordered_multimap(storage_type const & value, allocator_type const & alloc = {})
			: base_type{ value, alloc }
		{
			traits_type::sort_impl{}(*this);
		}
	
		explicit basic_ordered_multimap(storage_type && value, allocator_type const & alloc = {}) noexcept
			: base_type{ std::move(value), alloc }
		{
			traits_type::sort_impl{}(*this);
		}
	
		basic_ordered_multimap(self_type const & other, allocator_type const & alloc = {})
			: base_type{ other, alloc }
		{
		}
	
		basic_ordered_multimap(self_type && other, allocator_type const & alloc = {}) noexcept
			: base_type{ std::move(other), alloc }
		{
		}

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline bool binary_search(U && u) const
		{
			return std::binary_search(cbegin(), cend(), std::forward<U>(u), compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline iterator_pair equal_range(U && u)
		{
			return std::equal_range(begin(), end(), std::forward<U>(u), compare_impl{});
		}

		template <class U> inline const_iterator_pair equal_range(U && u) const
		{
			return std::equal_range(cbegin(), cend(), std::forward<U>(u), compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline iterator lower_bound(U && u)
		{
			return std::lower_bound(begin(), end(), std::forward<U>(u), compare_impl{});
		}

		template <class U> inline const_iterator lower_bound(U && u) const
		{
			return std::lower_bound(cbegin(), cend(), std::forward<U>(u), compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline iterator upper_bound(U && u)
		{
			return std::upper_bound(begin(), end(), std::forward<U>(u), compare_impl{});
		}

		template <class U> inline const_iterator upper_bound(U && u) const
		{
			return std::upper_bound(cbegin(), cend(), std::forward<U>(u), compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline  iterator find(U && u)
		{
			if (auto const it{ equal_range(std::forward<U>(u)) }; it.first != it.second)
			{
				return it.first;
			}
			return end();
		}

		template <class U> inline  const_iterator find(U && u) const
		{
			if (auto const it{ equal_range(std::forward<U>(u)) }; it.first != it.second)
			{
				return it.first;
			}
			return cend();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator insert(const_reference value)
		{
			return emplace(upper_bound(value.first), value);
		}

		inline iterator insert(value_type && value)
		{
			return emplace(upper_bound(std::move(value.first)), std::move(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Usings
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Ordered Map
	template <class Key, class Value
	> ML_USING ordered_map = typename _ML_DENSE basic_ordered_map<
		Key,
		Value,
		std::less<Key>,
		std::allocator<std::pair<Key, Value>>
	>;

	// Ordered Multimap
	template <class Key, class Value
	> ML_USING ordered_multimap = typename _ML_DENSE basic_ordered_multimap<
		Key,
		Value,
		std::less<Key>,
		std::allocator<std::pair<Key, Value>>
	>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DENSE_MAP_HPP_