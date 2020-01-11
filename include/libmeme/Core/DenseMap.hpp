#ifndef _ML_DENSE_MAP_HPP_
#define _ML_DENSE_MAP_HPP_

#include <libmeme/Core/Dense.hpp>

namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Basic Dense Map Traits
	template <class Key, class Value, class Comp, class Alloc, bool Multi
	> struct basic_map_traits : public basic_container_traits<std::pair<Key, Value>, Alloc>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename basic_container_traits<std::pair<Key, Value>, Alloc>;
		
		using key_type = typename Key;
		
		using mapped_type = typename Value;

		using compare_type = typename Comp;
		
		static constexpr bool multi{ Multi };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using value_type				= typename base_type::value_type;
		using allocator_type			= typename base_type::allocator_type;
		using storage_type				= typename base_type::storage_type;
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
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Basic Dense Ordered Map
	template <class Key, class Value, class Comp, class Alloc
	> struct basic_ordered_map : public basic_container<
		basic_map_traits<Key, Value, Comp, Alloc, false>
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type					= typename basic_ordered_map<Key, Value, Comp, Alloc>;
		using traits_type				= typename basic_map_traits<Key, Value, Comp, Alloc, false>;
		using base_type					= typename basic_container<traits_type>;
		using key_type					= typename traits_type::key_type;
		using mapped_type				= typename traits_type::mapped_type;
		using compare_type				= typename traits_type::compare_type;
		using value_type				= typename base_type::value_type;
		using allocator_type			= typename base_type::allocator_type;
		using storage_type				= typename base_type::storage_type;
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
			std::sort(begin(), end(), compare_type{});
			erase(std::unique(begin(), end(), compare_type{}), end());
		}

		template <class It> basic_ordered_map(It first, It last)
			: base_type{ first, last }
		{
			std::sort(begin(), end(), compare_type{});
			erase(std::unique(begin(), end(), compare_type{}), end());
		}

		explicit basic_ordered_map(storage_type const & value, allocator_type const & alloc = {})
			: base_type{ value, alloc }
		{
			std::sort(begin(), end(), compare_type{});
			erase(std::unique(begin(), end(), compare_type{}), end());
		}
	
		explicit basic_ordered_map(storage_type && value, allocator_type const & alloc = {}) noexcept
			: base_type{ std::move(value), alloc }
		{
			std::sort(begin(), end(), compare_type{});
			erase(std::unique(begin(), end(), compare_type{}), end());
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

		inline self_type & operator=(initializer_type init)
		{
			self_type temp{ init };
			swap(temp);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline bool contains(U && u) const
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

		template <class U> inline iterator find(U && u)
		{
			auto const it{ equal_range(std::forward<U>(u)) };
			return (it.first != it.second) ? it.first : end();
		}

		template <class U> inline const_iterator find(U && u) const
		{
			auto const it{ equal_range(std::forward<U>(u)) };
			return (it.first != it.second) ? it.first : cend();
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
			if (auto const it{ equal_range(std::move(value)) }; it.first != it.second)
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
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Basic Dense Ordered Multimap
	template <class Key, class Value, class Comp, class Alloc
	> struct basic_ordered_multimap : public basic_container<
		basic_map_traits<Key, Value, Comp, Alloc, true>
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
		using self_type					= typename basic_ordered_multimap<Key, Value, Comp, Alloc>;
		using traits_type				= typename basic_map_traits<Key, Value, Comp, Alloc, true>;
		using base_type					= typename basic_container<traits_type>;
		using key_type					= typename traits_type::key_type;
		using mapped_type				= typename traits_type::mapped_type;
		using compare_type				= typename traits_type::compare_type;
		using value_type				= typename base_type::value_type;
		using allocator_type			= typename base_type::allocator_type;
		using storage_type				= typename base_type::storage_type;
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
	
		basic_ordered_multimap() noexcept
			: base_type{}
		{
		}

		basic_ordered_multimap(initializer_type init)
			: base_type{ init }
		{
			std::sort(begin(), end(), compare_type{});
		}

		template <class It> basic_ordered_multimap(It first, It last)
			: base_type{ first, last }
		{
			std::sort(begin(), end(), compare_type{});
		}

		explicit basic_ordered_multimap(storage_type const & value, allocator_type const & alloc = {})
			: base_type{ value, alloc }
		{
			std::sort(begin(), end(), compare_type{});
		}
	
		explicit basic_ordered_multimap(storage_type && value, allocator_type const & alloc = {}) noexcept
			: base_type{ std::move(value), alloc }
		{
			std::sort(begin(), end(), compare_type{});
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

		inline self_type & operator=(initializer_type init)
		{
			self_type temp{ init };
			swap(temp);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline bool contains(U && u) const
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

		template <class U> inline iterator find(U && u)
		{
			auto const it{ equal_range(std::forward<U>(u)) };
			return (it.first != it.second) ? it.first : end();
		}

		template <class U> inline const_iterator find(U && u) const
		{
			auto const it{ equal_range(std::forward<U>(u)) };
			return (it.first != it.second) ? it.first : cend();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator insert(const_reference value)
		{
			return m_storage.emplace(upper_bound(value), value);
		}

		inline iterator insert(value_type && value)
		{
			return m_storage.emplace(upper_bound(std::move(value)), std::move(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Key Compare
	template <
		template <class> class Comp,
		template <class, class> class Pair, class Key, class Val
	> struct key_compare
	{
		constexpr bool operator()(Key const & lhs, Pair<Key, Val> const & rhs) const
		{
			return Comp<Key>{}(lhs, rhs.first);
		}

		constexpr bool operator()(Pair<Key, Val> const & lhs, Key const & rhs) const
		{
			return Comp<Key>{}(lhs.first, rhs);
		}

		constexpr bool operator()(Pair<Key, Val> const & lhs, Pair<Key, Val> const & rhs) const
		{
			return Comp<Key>{}(lhs.first, rhs.first);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Ordered Map
	template <class Key, class Value
	> ML_USING ordered_map = typename _ML_DENSE basic_ordered_map<
		Key,
		Value,
		key_compare<std::less, std::pair, Key, Value>,
		std::allocator<std::pair<Key, Value>>
	>;

	// Ordered Multimap
	template <class Key, class Value
	> ML_USING ordered_multimap = typename _ML_DENSE basic_ordered_multimap<
		Key,
		Value,
		key_compare<std::less, std::pair, Key, Value>,
		std::allocator<std::pair<Key, Value>>
	>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DENSE_MAP_HPP_