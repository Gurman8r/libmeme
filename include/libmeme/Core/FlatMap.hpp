#ifndef _ML_FLAT_MAP_HPP_
#define _ML_FLAT_MAP_HPP_

#include <libmeme/Core/FlatSet.hpp>

// WIP - DO NOT USE

namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// FLAT MAP TRAITS
	template <class Key, class Value, class Comp, bool Multi
	> struct flat_map_traits
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using key_type				= typename Key;
		using mapped_type			= typename Value;
		using compare_type			= typename Comp;

		static constexpr bool multi{ Multi };

		using key_storage			= typename ds::set<key_type, compare_type>;
		using key_iterator			= typename key_storage::iterator;
		using key_const_iterator	= typename key_storage::const_iterator;

		using mapped_storage		= typename std::vector<mapped_type>;
		using mapped_iterator		= typename mapped_storage::iterator;
		using mapped_const_iterator	= typename mapped_storage::const_iterator;

		using iterator_pair			= typename std::pair<key_iterator, mapped_iterator>;
		using const_iterator_pair	= typename std::pair<key_const_iterator, mapped_const_iterator>;

		using difference_type		= typename ptrdiff_t;
		using size_type				= typename size_t;
		using storage_type			= typename std::pair<key_storage, mapped_storage>;
		using pair_type				= typename std::pair<key_type, mapped_type>;
		using initializer_type		= typename std::initializer_list<pair_type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// BASIC FLAT MAP
	template <class Traits
	> struct basic_flat_map
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using traits_type			= typename Traits;
		using self_type				= typename basic_flat_map<traits_type>;
		using key_type				= typename traits_type::key_type;
		using mapped_type			= typename traits_type::mapped_type;
		using compare_type			= typename traits_type::compare_type;
		
		using key_storage			= typename traits_type::key_storage;
		using key_iterator			= typename traits_type::key_iterator;
		using key_const_iterator	= typename traits_type::key_const_iterator;
		
		using mapped_storage		= typename traits_type::mapped_storage;
		using mapped_iterator		= typename traits_type::mapped_iterator;
		using mapped_const_iterator	= typename traits_type::mapped_const_iterator;

		using iterator_pair			= typename traits_type::iterator_pair;
		using const_iterator_pair	= typename traits_type::const_iterator_pair;
		
		using difference_type		= typename traits_type::difference_type;
		using size_type				= typename traits_type::size_type;
		using storage_type			= typename traits_type::storage_type;
		using pair_type				= typename traits_type::pair_type;
		using initializer_type		= typename traits_type::initializer_type;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_flat_map() noexcept
			: m_storage{}
		{
		}

		explicit basic_flat_map(storage_type const & value)
			: m_storage{ value }
		{
		}

		explicit basic_flat_map(storage_type && value) noexcept
			: m_storage{ std::move(value) }
		{
		}

		basic_flat_map(self_type const & other)
			: m_storage{ other.m_storage }
		{
		}

		basic_flat_map(self_type && other) noexcept
			: m_storage{ std::move(other.m_storage) }
		{
		}

		virtual ~basic_flat_map() noexcept {}

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

		inline auto keys() noexcept -> key_storage & { return m_storage.first; }

		inline auto keys() const noexcept -> key_storage const & { return m_storage.first; }

		inline auto values() noexcept -> mapped_storage & { return m_storage.second; }

		inline auto values() const noexcept -> mapped_storage const & { return m_storage.second; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void clear() noexcept
		{
			keys().clear();
			values().clear();
		}

		inline void reserve(size_type const count)
		{
			keys().reserve(count);
			values().reserve(count);
		}

		inline void resize(size_type const count)
		{
			keys().resize(count);
			values().resize(count);
		}

		inline void shrink_to_fit()
		{
			keys().shrink_to_fit();
			values().shrink_to_fit();
		}

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline size_type get_index(key_const_iterator value) const
		{
			return static_cast<size_type>(std::distance(keys().cbegin(), value));
		}

		inline mapped_iterator get_iterator(key_const_iterator value)
		{
			return values().begin() + get_index(value);
		}

		inline mapped_const_iterator get_iterator(key_const_iterator value) const
		{
			return values().cbegin() + get_index(value);
		}

		inline mapped_type & get_mapped(key_const_iterator value)
		{
			return *get_iterator(value);
		}

		inline mapped_type const & get_mapped(key_const_iterator value) const
		{
			return *get_iterator(value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// FLAT MAP
	template <class Key, class Value, class Comp
	> struct flat_map : public basic_flat_map<
		flat_map_traits<Key, Value, Comp, false>
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type				= typename flat_map<Key, Value, Comp>;
		using base_type				= typename basic_flat_map<flat_map_traits<Key, Value, Comp, false>>;
		using traits_type			= typename base_type::traits_type;
		using compare_type			= typename base_type::compare_type;
		
		using key_type				= typename base_type::key_type;
		using key_storage			= typename base_type::key_storage;
		using key_iterator			= typename base_type::key_iterator;
		using key_const_iterator	= typename base_type::key_const_iterator;
		
		using mapped_type			= typename base_type::mapped_type;
		using mapped_storage			= typename base_type::mapped_storage;
		using mapped_iterator		= typename base_type::mapped_iterator;
		using mapped_const_iterator	= typename base_type::mapped_const_iterator;

		using iterator_pair			= typename base_type::iterator_pair;
		using const_iterator_pair	= typename base_type::const_iterator_pair;
		
		using difference_type		= typename base_type::difference_type;
		using size_type				= typename base_type::size_type;
		using storage_type			= typename base_type::storage_type;
		using pair_type				= typename base_type::pair_type;
		using initializer_type		= typename base_type::initializer_type;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		flat_map() noexcept
			: base_type{}
		{
		}

		flat_map(initializer_type init)
			: base_type{}
		{
			for (auto const & elem : init)
			{
				insert(elem);
			}
		}

		explicit flat_map(storage_type const & value)
			: base_type{ value }
		{
		}

		explicit flat_map(storage_type && value) noexcept
			: base_type{ std::move(value) }
		{
		}

		flat_map(self_type const & other)
			: base_type{ other.m_keys, other.m_values }
		{
		}

		flat_map(self_type && other) noexcept
			: base_type{ std::move(other.m_keys), std::move(other.m_values) }
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

		inline std::optional<iterator_pair> find(key_type const & key)
		{
			if (auto it = keys().find(key); it != keys().end())
			{
				return std::make_optional(std::make_pair(it, get_iterator(it)));
			}
			return std::nullopt;
		}

		inline std::optional<mapped_const_iterator> find(key_type const & key) const
		{
			if (auto it = keys().find(key); it != keys().cend())
			{
				return std::make_optional(std::make_pair(it, get_iterator(it)));
			}
			return std::nullopt;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::pair<iterator_pair, bool> insert(pair_type const & value)
		{
			if (auto it{ keys().find(value.first) }; it == keys().end())
			{
				return std::make_pair(_unchecked_insert(value), true);
			}
			else
			{
				return std::make_pair(std::make_pair(it, get_iterator(it)), false);
			}
		}

		inline std::pair<iterator_pair, bool> insert(pair_type && value)
		{
			if (auto it{ keys().find(std::move(value.first)) }; it == keys().end())
			{
				return std::make_pair(_unchecked_insert(std::move(value)), true);
			}
			else
			{
				return std::make_pair(std::make_pair(it, get_iterator(it)), false);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::optional<iterator_pair> try_insert(pair_type const & value)
		{
			if (auto it{ keys().find(value.first) }; it == keys().end())
			{
				return std::make_optional(_unchecked_insert(value));
			}
			else
			{
				return std::nullopt;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Callable
		> inline decltype(auto) insert_then(pair_type const & value, Callable callable)
		{
			auto result = insert(value); // pair<iterator_pair, bool>
			return std::invoke(callable, result.first, result.second);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline mapped_type & at(key_type const & key)
		{
			if (auto it{ keys().find(key) }; it != keys().end())
			{
				return get_mapped(it);
			}
			else
			{
				return *_unchecked_insert(pair_type{ key, mapped_type{} }).second;
			}
		}

		inline mapped_type const & at(key_type const & key) const
		{
			if (auto it{ keys().find(key) }; it == keys().end())
			{
				ML_THROW std::out_of_range("invalid flat_map<K, V> key");
			}
			else
			{
				return get_mapped(it);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline mapped_type & operator[](key_type const & key)
		{
			return at(key);
		}

		inline mapped_type const & operator[](key_type const & key) const
		{
			return at(key);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		inline iterator_pair _unchecked_insert(pair_type const & value)
		{
			auto it = keys().insert(value.first).first;

			if (keys().size() != values().size())
			{
				values().resize(keys().size());
			}

			get_mapped(it) = value.second;

			return std::make_pair(it, get_iterator(it));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Key, class Value
	> ML_USING map = typename _ML_DS flat_map<Key, Value, std::less<Key>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FLAT_MAP_HPP_