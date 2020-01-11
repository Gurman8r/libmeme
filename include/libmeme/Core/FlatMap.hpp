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

		using key_type			= typename Key;
		using mapped_type		= typename Value;
		using compare_type		= typename Comp;

		static constexpr bool multi{ Multi };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using difference_type		= typename ptrdiff_t;
		using size_type				= typename size_t;

		using key_storage			= typename ds::set<key_type>;
		using key_iterator			= typename key_storage::iterator;
		using key_const_iterator	= typename key_storage::const_iterator;

		using mapped_storage		= typename std::vector<mapped_type>;
		using mapped_iterator		= typename mapped_storage::iterator;
		using mapped_const_iterator	= typename mapped_storage::const_iterator;

		using storage_type			= typename std::pair<key_storage, mapped_storage>;
		using value_type			= typename std::pair<key_type, mapped_type>;
		using initializer_type		= typename std::initializer_list<value_type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// FLAT MAP CONST ITERATOR
	template <class MyMap
	> struct flat_map_const_iterator
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using iterator_category		= typename std::random_access_iterator_tag;
		using parent_type			= typename MyMap;
		using self_type				= typename flat_map_const_iterator<parent_type>;
		using size_type				= typename parent_type::size_type;
		using difference_type		= typename parent_type::difference_type;
		using storage_type			= typename parent_type::storage_type;

		using key_storage			= typename storage_type::first_type;
		using mapped_storage		= typename storage_type::second_type;

		using key_type				= typename key_storage::value_type;
		using mapped_type			= typename mapped_storage::value_type;

		using key_const_reference	= typename key_storage::const_reference;
		using mapped_const_reference= typename mapped_storage::const_reference;

		using key_const_pointer		= typename key_storage::const_pointer;
		using mapped_const_pointer	= typename mapped_storage::const_pointer;

		using key_const_iterator	= typename key_storage::const_iterator;
		using mapped_const_iterator	= typename mapped_storage::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		flat_map_const_iterator() noexcept
			: m_pair{}, m_parent{ nullptr }
		{
		}

		flat_map_const_iterator(std::pair<key_const_iterator, mapped_const_iterator> pair, parent_type const * parent) noexcept
			: m_pair{ pair }, m_parent{ parent }
		{
		}

		~flat_map_const_iterator() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::pair<key_const_reference, mapped_const_reference> operator*() const
		{
			return std::make_pair(*m_pair.first, *m_pair.second);
		}
		
		inline std::pair<key_const_iterator, mapped_const_iterator> const * operator->() const
		{
			return &m_pair;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool operator==(self_type const & other) const
		{
			return !(*this < other) && !(other < *this);
		}

		inline bool operator!=(self_type const & other) const
		{
			return !(*this == other);
		}

		inline bool operator<(self_type const & other) const
		{
			return m_pair.first < other.m_pair.first;
		}

		inline bool operator<=(self_type const & other) const
		{
			return (*this < other) || (*this == other);
		}

		inline bool operator>(self_type const & other) const
		{
			return !(*this < other);
		}

		inline bool operator>=(self_type const & other) const
		{
			return !(*this < other) || (*this == other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator++()
		{
			++m_pair.first;
			++m_pair.second;
			return (*this);
		}

		inline self_type & operator--()
		{
			--m_pair.first;
			--m_pair.second;
			return (*this);
		}

		inline self_type operator++(int)
		{
			self_type temp{ *this };
			++(*this);
			return temp;
		}

		inline self_type operator--(int)
		{
			self_type temp{ *this };
			--(*this);
			return temp;
		}

		inline self_type & operator+=(difference_type const offset)
		{
			m_pair.first += offset;
			m_pair.second += offset;
			return (*this);
		}

		inline self_type & operator-=(difference_type const offset)
		{
			m_pair.first -= offset;
			m_pair.second -= offset;
			return (*this);
		}

		inline self_type & operator+(difference_type const offset)
		{
			self_type temp{ *this };
			return temp += offset;
		}

		inline self_type & operator-(difference_type const offset)
		{
			self_type temp{ *this };
			return temp -= offset;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto const & pair() const noexcept { return m_pair; }

		inline auto parent() const noexcept -> parent_type const * { return m_parent; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		std::pair<key_const_iterator, mapped_const_iterator> m_pair;

		parent_type const * m_parent;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// FLAT MAP ITERATOR (broken)
	template <class MyMap
	> struct flat_map_iterator : public flat_map_const_iterator<MyMap>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using iterator_category		= typename std::random_access_iterator_tag;
		using parent_type			= typename MyMap;
		using self_type				= typename flat_map_iterator<parent_type>;
		using base_type				= typename flat_map_const_iterator<MyMap>;
		using size_type				= typename parent_type::size_type;
		using difference_type		= typename parent_type::difference_type;
		using storage_type			= typename parent_type::storage_type;

		using key_storage			= typename storage_type::first_type;
		using mapped_storage		= typename storage_type::second_type;

		using key_type				= typename key_storage::value_type;
		using mapped_type			= typename mapped_storage::value_type;

		using key_pointer			= typename key_storage::pointer;
		using mapped_pointer		= typename mapped_storage::pointer;

		using key_reference			= typename key_storage::reference;
		using mapped_reference		= typename mapped_storage::reference;

		using key_iterator			= typename key_storage::iterator;
		using mapped_iterator		= typename mapped_storage::iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::pair<key_reference, mapped_reference> operator*() const
		{
			static_assert(0, "WIP");
		}

		inline std::pair<key_iterator, mapped_iterator> * operator->() const
		{
			static_assert(0, "WIP");
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool operator==(self_type const & other) const
		{
			return !(*this < other) && !(other < *this);
		}

		inline bool operator!=(self_type const & other) const
		{
			return !(*this == other);
		}

		inline bool operator<(self_type const & other) const
		{
			return m_pair.first < other.m_pair.first;
		}

		inline bool operator<=(self_type const & other) const
		{
			return (*this < other) || (*this == other);
		}

		inline bool operator>(self_type const & other) const
		{
			return !(*this < other);
		}

		inline bool operator>=(self_type const & other) const
		{
			return !(*this < other) || (*this == other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator++()
		{
			base_type::operator++();
			return (*this);
		}

		inline self_type & operator--()
		{
			base_type::operator--();
			return (*this);
		}

		inline self_type operator++(int)
		{
			self_type temp{ *this };
			base_type::operator++();
			return temp;
		}

		inline self_type operator--(int)
		{
			self_type temp{ *this };
			base_type::operator--();
			return temp;
		}

		inline self_type & operator+=(difference_type const offset)
		{
			base_type::operator+=(offset);
			return (*this);
		}

		inline self_type & operator-=(difference_type const offset)
		{
			base_type::operator-=(offset);
			return (*this);
		}

		inline self_type & operator+(difference_type const offset)
		{
			self_type temp{ *this };
			return temp += offset;
		}

		inline self_type & operator-(difference_type const offset)
		{
			self_type temp{ *this };
			return temp -= offset;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// BASIC FLAT MAP
	template <class Traits
	> struct basic_flat_map
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using traits_type				= typename Traits;
		using self_type					= typename basic_flat_map<traits_type>;
		using key_type					= typename traits_type::key_type;
		using mapped_type				= typename traits_type::mapped_type;
		using compare_type				= typename traits_type::compare_type;
		using difference_type			= typename traits_type::difference_type;
		using size_type					= typename traits_type::size_type;
		using storage_type				= typename traits_type::storage_type;
		using value_type				= typename traits_type::value_type;
		using initializer_type			= typename traits_type::initializer_type;

		using key_storage				= typename traits_type::key_storage;
		using mapped_storage			= typename traits_type::mapped_storage;

		using key_pointer				= typename key_storage::pointer;
		using mapped_pointer			= typename mapped_storage::pointer;

		using key_reference				= typename key_storage::reference;
		using mapped_reference			= typename mapped_storage::reference;

		using key_const_reference		= typename key_storage::const_reference;
		using mapped_const_reference	= typename mapped_storage::const_reference;

		using key_const_pointer			= typename key_storage::const_pointer;
		using mapped_const_pointer		= typename mapped_storage::const_pointer;

		using key_iterator				= typename key_storage::iterator;
		using mapped_iterator			= typename mapped_storage::iterator;

		using key_const_iterator		= typename key_storage::const_iterator;
		using mapped_const_iterator		= typename mapped_storage::const_iterator;

		using iterator					= typename flat_map_iterator<self_type>;
		using const_iterator			= typename flat_map_const_iterator<self_type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_flat_map() noexcept
			: m_storage{}
		{
		}

		basic_flat_map(initializer_type init)
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

		inline self_type & operator=(initializer_type init)
		{
			self_type temp{ init };
			swap(temp);
			return (*this);
		}

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

		inline auto keys() noexcept -> key_storage & { return m_storage.first; }

		inline auto keys() const noexcept -> key_storage const & { return m_storage.first; }

		inline auto values() noexcept -> mapped_storage & { return m_storage.second; }

		inline auto values() const noexcept -> mapped_storage const & { return m_storage.second; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto _begin() noexcept -> std::pair<key_iterator, mapped_iterator>
		{
			return std::make_pair(m_storage.first.begin(), m_storage.second.begin());
		}

		inline auto _end() noexcept -> std::pair<key_iterator, mapped_iterator>
		{
			return std::make_pair(keys().end(), values().end());
		}

		inline auto _cbegin() const noexcept -> std::pair<key_const_iterator, mapped_const_iterator>
		{
			return std::make_pair(keys().cbegin(), values().cbegin());
		}

		inline auto _cend() const noexcept -> std::pair<key_const_iterator, mapped_const_iterator>
		{
			return std::make_pair(keys().cend(), values().cend());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		//inline auto begin() noexcept { return iterator{ _begin(), this }; }

		//inline auto end() noexcept { return iterator{ _end(), this }; }

		inline auto begin() const noexcept { return const_iterator{ _cbegin(), this }; }

		inline auto end() const noexcept { return const_iterator{ _cend(), this }; }

		inline auto cbegin() const noexcept { return const_iterator{ _cbegin(), this }; }

		inline auto cend() const noexcept { return const_iterator{ _cend(), this }; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class It> inline size_type index_of(It it) const
		{
			if constexpr (std::is_same_v<It, const_iterator>)
			{
				return static_cast<size_type>(std::distance(_cbegin().first, it.pair().first));
			}
			else
			{
				return static_cast<size_type>(std::distance(_cbegin().first, it));
			}
		}

		inline mapped_type const & at(key_type const & key) const
		{
			return values().at(index_of(keys().find(key)));
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

		using self_type					= typename flat_map<Key, Value, Comp>;
		using base_type					= typename basic_flat_map<flat_map_traits<Key, Value, Comp, false>>;
		using traits_type				= typename base_type::traits_type;
		using key_type					= typename base_type::key_type;
		using mapped_type				= typename base_type::mapped_type;
		using compare_type				= typename base_type::compare_type;
		using storage_type				= typename base_type::storage_type;
		using difference_type			= typename base_type::difference_type;
		using size_type					= typename base_type::size_type;
		using value_type				= typename base_type::value_type;
		using initializer_type			= typename base_type::initializer_type;
		using const_iterator			= typename base_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		flat_map() noexcept
			: base_type{}
		{
		}

		flat_map(initializer_type init)
			: base_type{ init }
		{
			sort_internal();
		}

		explicit flat_map(storage_type const & value)
			: base_type{ value }
		{
			sort_internal();
		}

		explicit flat_map(storage_type && value) noexcept
			: base_type{ std::move(value) }
		{
			sort_internal();
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

	private:
		inline void sort_internal()
		{
			//...
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Key, class Value
	> ML_USING map = typename _ML_DS flat_map<Key, Value, std::less<Key>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FLAT_MAP_HPP_