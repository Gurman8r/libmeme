#ifndef _ML_PAIR_MAP_HPP_
#define _ML_PAIR_MAP_HPP_

#include <libmeme/Core/Core.hpp>

namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// KEY COMPARE
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

	// BASIC PAIR MAP TRAITS
	template <class Key, class Value, class Comp, class Alloc, bool Multi
	> struct basic_pair_map_traits
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using key_type = typename Key;
		
		using mapped_type = typename Value;

		using value_type = typename std::pair<Key, Value>;

		using compare_type = typename Comp;

		using allocator_type = typename Alloc;
		
		static constexpr bool multi{ Multi };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	// BASIC PAIR MAP
	template <class Traits
	> struct basic_pair_map
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using traits_type				= typename Traits;
		using self_type					= typename basic_pair_map<traits_type>;
		using key_type					= typename traits_type::key_type;
		using mapped_type				= typename traits_type::mapped_type;
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

		basic_pair_map() noexcept : m_storage{} {}

		~basic_pair_map() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class It> basic_pair_map(It first, It last)
			: m_storage{ first, last }
		{
		}

		basic_pair_map(initializer_type init)
			: m_storage{ init }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit basic_pair_map(storage_type const & value)
			: m_storage{ value }
		{
		}

		explicit basic_pair_map(storage_type && value) noexcept
			: m_storage{ std::move(value) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit basic_pair_map(storage_type const & value, allocator_type const & alloc)
			: m_storage{ value, alloc }
		{
		}

		explicit basic_pair_map(storage_type && value, allocator_type const & alloc) noexcept
			: m_storage{ std::move(value), alloc }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_pair_map(self_type const & other)
			: m_storage{ other.m_storage }
		{
		}

		basic_pair_map(self_type && other) noexcept
			: m_storage{ std::move(other.m_storage) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_pair_map(self_type const & other, allocator_type const & alloc)
			: m_storage{ other.m_storage, alloc }
		{
		}

		basic_pair_map(self_type && other, allocator_type const & alloc) noexcept
			: m_storage{ std::move(other.m_storage), alloc }
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

		ML_NODISCARD inline bool contains(key_type const & key) const
		{
			return std::binary_search(cbegin(), cend(), key, compare_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline iterator_pair equal_range(key_type const & key)
		{
			return std::equal_range(begin(), end(), key, compare_type{});
		}

		ML_NODISCARD inline const_iterator_pair equal_range(key_type const & key) const
		{
			return std::equal_range(cbegin(), cend(), key, compare_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline iterator lower_bound(key_type const & key)
		{
			return std::lower_bound(begin(), end(), key, compare_type{});
		}

		ML_NODISCARD inline const_iterator lower_bound(key_type const & key) const
		{
			return std::lower_bound(cbegin(), cend(), key, compare_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline iterator upper_bound(key_type const & key)
		{
			return std::upper_bound(begin(), end(), key, compare_type{});
		}

		ML_NODISCARD inline const_iterator upper_bound(key_type const & key) const
		{
			return std::upper_bound(cbegin(), cend(), key, compare_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline iterator find(const_reference value)
		{
			return std::find(begin(), end(), value);
		}

		ML_NODISCARD inline const_iterator find(const_reference value) const
		{
			return std::find(cbegin(), cend(), value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto back() noexcept -> reference { return m_storage.back(); }

		ML_NODISCARD inline auto back() const noexcept -> const_reference { return m_storage.back(); }

		ML_NODISCARD inline auto begin() noexcept -> iterator { return m_storage.begin(); }

		ML_NODISCARD inline auto begin() const noexcept -> const_iterator { return m_storage.begin(); }

		ML_NODISCARD inline auto capacity() const noexcept -> size_type { return m_storage.capacity(); }

		ML_NODISCARD inline auto cbegin() const noexcept -> const_iterator { return m_storage.cbegin(); }

		ML_NODISCARD inline auto cend() const noexcept -> const_iterator { return m_storage.cend(); }

		ML_NODISCARD inline auto crbegin() const noexcept -> const_reverse_iterator { return m_storage.crbegin(); }

		ML_NODISCARD inline auto crend() const noexcept -> const_reverse_iterator { return m_storage.crend(); }

		ML_NODISCARD inline auto data() noexcept -> pointer { return m_storage.data(); }

		ML_NODISCARD inline auto data() const noexcept -> const_pointer { return m_storage.data(); }

		ML_NODISCARD inline bool empty() const noexcept { return m_storage.empty(); }

		ML_NODISCARD inline auto end() noexcept -> iterator { return m_storage.end(); }

		ML_NODISCARD inline auto end() const noexcept -> const_iterator { return m_storage.end(); }

		ML_NODISCARD inline auto front() noexcept -> reference { return m_storage.front(); }

		ML_NODISCARD inline auto front() const noexcept -> const_reference { return m_storage.front(); }

		ML_NODISCARD inline auto get_allocator() const noexcept -> allocator_type { return m_storage.get_allocator(); }

		ML_NODISCARD inline auto max_size() const noexcept -> size_type { return m_storage.max_size(); }

		ML_NODISCARD inline auto rbegin() noexcept -> reverse_iterator { return m_storage.rbegin(); }

		ML_NODISCARD inline auto rbegin() const noexcept -> const_reverse_iterator { return m_storage.rbegin(); }

		ML_NODISCARD inline auto rend() noexcept -> reverse_iterator { return m_storage.rend(); }

		ML_NODISCARD inline auto rend() const noexcept -> const_reverse_iterator { return m_storage.rend(); }

		ML_NODISCARD inline auto size() const noexcept -> size_type { return m_storage.size(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		storage_type m_storage;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// PAIR MAP
	template <class Key, class Value,
		class Comp = key_compare<std::less, std::pair, Key, Value>,
		class Alloc = std::allocator<std::pair<Key, Value>>
	> struct pair_map final : public basic_pair_map<
		basic_pair_map_traits<Key, Value, Comp, Alloc, false>
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type					= typename pair_map<Key, Value, Comp, Alloc>;
		using base_type					= typename basic_pair_map<basic_pair_map_traits<Key, Value, Comp, Alloc, false>>;
		using traits_type				= typename base_type::traits_type;
		using key_type					= typename base_type::key_type;
		using mapped_type				= typename base_type::mapped_type;
		using value_type				= typename base_type::value_type;
		using compare_type				= typename base_type::compare_type;
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

		pair_map() noexcept : base_type{} {}

		~pair_map() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		pair_map(initializer_type init)
			: base_type{ init }
		{
			sort();
		}

		template <class It> pair_map(It first, It last)
			: base_type{ first, last }
		{
			sort();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit pair_map(storage_type const & other)
			: base_type{ other }
		{
			sort();
		}

		explicit pair_map(storage_type && other) noexcept
			: base_type{ std::move(other) }
		{
			sort();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit pair_map(storage_type const & other, allocator_type const & alloc)
			: base_type{ other, alloc }
		{
			sort();
		}

		explicit pair_map(storage_type && other, allocator_type const & alloc) noexcept
			: base_type{ std::move(other), alloc }
		{
			sort();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		pair_map(self_type const & other)
			: base_type{ other.m_storage }
		{
		}

		pair_map(self_type && other) noexcept
			: base_type{ std::move(other.m_storage) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		pair_map(self_type const & other, allocator_type const & alloc)
			: base_type{ other.m_storage, alloc }
		{
		}

		pair_map(self_type && other, allocator_type const & alloc) noexcept
			: base_type{ std::move(other.m_storage), alloc }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		inline self_type operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline self_type operator=(initializer_type init)
		{
			self_type temp{ init };
			swap(temp);
			return (*this);
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

		ML_NODISCARD inline mapped_type & at(key_type const & key)
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

		ML_NODISCARD inline mapped_type & at(key_type && key)
		{
			if (auto const it{ equal_range(std::move(key)) }; it.first != it.second)
			{
				return it.first->second;
			}
			else
			{
				return m_storage.emplace(it.second, value_type{ std::move(key), mapped_type{} })->second;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline mapped_type & operator[](key_type const & key)
		{
			return at(key);
		}

		ML_NODISCARD inline mapped_type & operator[](key_type && key)
		{
			return at(std::move(key));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		inline void sort()
		{
			std::sort(base_type::begin(), base_type::end(), compare_type{});

			base_type::erase(std::unique(base_type::begin(), base_type::end(), compare_type{}), base_type::end());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// PAIR MULTISET
	template <class Key, class Value,
		class Comp = key_compare<std::less, std::pair, Key, Value>,
		class Alloc = std::allocator<std::pair<Key, Value>>
	> struct pair_multimap final : public basic_pair_map<
		basic_pair_map_traits<Key, Value, Comp, Alloc, true>
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type					= typename pair_multimap<Key, Value, Comp, Alloc>;
		using base_type					= typename basic_pair_map<basic_pair_map_traits<Key, Value, Comp, Alloc, true>>;
		using traits_type				= typename base_type::traits_type;
		using key_type					= typename base_type::key_type;
		using mapped_type				= typename base_type::mapped_type;
		using value_type				= typename base_type::value_type;
		using compare_type				= typename base_type::compare_type;
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

		pair_multimap() noexcept : base_type{} {}

		~pair_multimap() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		pair_multimap(initializer_type init)
			: base_type{ init }
		{
			sort();
		}

		template <class It> pair_multimap(It first, It last)
			: base_type{ first, last }
		{
			sort();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit pair_multimap(storage_type const & other)
			: base_type{ other }
		{
			sort();
		}

		explicit pair_multimap(storage_type && other) noexcept
			: base_type{ std::move(other) }
		{
			sort();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit pair_multimap(storage_type const & other, allocator_type const & alloc)
			: base_type{ other, alloc }
		{
			sort();
		}

		explicit pair_multimap(storage_type && other, allocator_type const & alloc) noexcept
			: base_type{ std::move(other), alloc }
		{
			sort();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		pair_multimap(self_type const & other)
			: base_type{ other.m_storage }
		{
		}

		pair_multimap(self_type && other) noexcept
			: base_type{ std::move(other.m_storage) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		pair_multimap(self_type const & other, allocator_type const & alloc)
			: base_type{ other.m_storage, alloc }
		{
		}

		pair_multimap(self_type && other, allocator_type const & alloc) noexcept
			: base_type{ std::move(other.m_storage), alloc }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		inline self_type operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		inline self_type operator=(initializer_type init)
		{
			self_type temp{ init };
			swap(temp);
			return (*this);
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

	private:
		inline void sort() noexcept
		{
			std::sort(base_type::begin(), base_type::end(), compare_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PAIR_MAP_HPP_