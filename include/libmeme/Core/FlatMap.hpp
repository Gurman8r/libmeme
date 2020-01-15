#ifndef _ML_FLAT_MAP_HPP_
#define _ML_FLAT_MAP_HPP_

#include <libmeme/Core/FlatSet.hpp>

namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Basic Flat Map Traits
	template <class Key, class Value, class Compare, template <class> class Alloc, bool Multi
	> struct basic_flat_map_traits
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using key_type = typename Key;
		
		using value_type = typename Value;
		
		using compare_type = typename Compare;

		template <class T
		> using allocator_type = typename Alloc<T>;

		static constexpr bool multi{ Multi };

		using difference_type = typename ptrdiff_t;
		
		using size_type = typename size_t;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using key_allocator					= typename allocator_type<key_type>;
		using key_storage					= typename ds::flat_set<key_type, compare_type, key_allocator>;
		using key_iterator					= typename key_storage::iterator;
		using const_key_iterator			= typename key_storage::const_iterator;
		using reverse_key_iterator			= typename key_storage::reverse_iterator;
		using const_reverse_key_iterator	= typename key_storage::const_reverse_iterator;
		
		using value_allocator				= typename allocator_type<value_type>;
		using value_storage					= typename std::vector<value_type, value_allocator>;
		using value_iterator				= typename value_storage::iterator;
		using const_value_iterator			= typename value_storage::const_iterator;
		using reverse_value_iterator		= typename value_storage::reverse_iterator;
		using const_reverse_value_iterator	= typename value_storage::const_reverse_iterator;
		
		using storage_type					= typename std::pair<key_storage, value_storage>;
		using initializer_type				= typename std::initializer_list<std::pair<key_type, value_type>>;
		using allocator_pair				= typename std::pair<key_allocator, value_allocator>;
		using iterator_pair					= typename std::pair<key_iterator, value_iterator>;
		using const_iterator_pair			= typename std::pair<const_key_iterator, const_value_iterator>;
		using reverse_iterator_pair			= typename std::pair<reverse_key_iterator, reverse_value_iterator>;
		using const_reverse_iterator_pair	= typename std::pair<const_reverse_key_iterator, const_reverse_value_iterator>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Basic Flat Map
	template <class Traits
	> struct basic_flat_map
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using traits_type					= typename Traits;
		using self_type						= typename basic_flat_map<traits_type>;
		using key_type						= typename traits_type::key_type;
		using value_type					= typename traits_type::value_type;
		using compare_type					= typename traits_type::compare_type;
		using difference_type				= typename traits_type::difference_type;
		using size_type						= typename traits_type::size_type;
		
		using key_allocator					= typename traits_type::key_allocator;
		using key_storage					= typename traits_type::key_storage;
		using key_iterator					= typename traits_type::key_iterator;
		using const_key_iterator			= typename traits_type::const_key_iterator;
		using reverse_key_iterator			= typename traits_type::reverse_key_iterator;
		using const_reverse_key_iterator	= typename traits_type::const_reverse_key_iterator;
		
		using value_allocator				= typename traits_type::value_allocator;
		using value_storage					= typename traits_type::value_storage;
		using value_iterator				= typename traits_type::value_iterator;
		using const_value_iterator			= typename traits_type::const_value_iterator;
		using reverse_value_iterator		= typename traits_type::reverse_value_iterator;
		using const_reverse_value_iterator	= typename traits_type::const_reverse_value_iterator;

		using storage_type					= typename traits_type::storage_type;
		using initializer_type				= typename traits_type::initializer_type;
		using allocator_pair				= typename traits_type::allocator_pair;
		using iterator_pair					= typename traits_type::iterator_pair;
		using const_iterator_pair			= typename traits_type::const_iterator_pair;
		using reverse_iterator_pair			= typename traits_type::reverse_iterator_pair;
		using const_reverse_iterator_pair	= typename traits_type::const_reverse_iterator_pair;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_flat_map() noexcept : m_storage{} {}

		~basic_flat_map() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit basic_flat_map(storage_type const & value)
			: m_storage{ value }
		{
		}

		explicit basic_flat_map(storage_type && value) noexcept
			: m_storage{ std::move(value) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit basic_flat_map(key_storage const & keys, value_storage const & values)
			: self_type{ { keys, values } }
		{
		}

		explicit basic_flat_map(key_storage && keys, value_storage && values) noexcept
			: self_type{ { std::move(keys), std::move(values) } }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit basic_flat_map(key_storage const & keys, value_storage const & values, allocator_pair const & alloc)
			: self_type{ { key_storage{ keys, alloc.first }, value_storage{ values, alloc.second } } }
		{
		}

		explicit basic_flat_map(key_storage && keys, value_storage && values, allocator_pair const & alloc) noexcept
			: self_type{ { key_storage{ std::move(keys), alloc.first }, value_storage{ std::move(values), alloc.second } } }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit basic_flat_map(storage_type const & value, allocator_pair const & alloc)
			: self_type{ value.first, value.second, alloc }
		{
		}

		explicit basic_flat_map(storage_type && value, allocator_pair const & alloc) noexcept
			: self_type{ std::move(value.first), std::move(value.second), alloc }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_flat_map(self_type const & other)
			: self_type{ other.m_storage }
		{
		}

		basic_flat_map(self_type && other) noexcept
			: self_type{ std::move(other.m_storage) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_flat_map(self_type const & other, allocator_pair const & alloc)
			: self_type{ other.m_storage, alloc }
		{
		}

		basic_flat_map(self_type && other, allocator_pair const & alloc) noexcept
			: self_type{ std::move(other.m_storage), alloc }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			this->swap(temp);
			return (*this);
		}

		inline self_type & operator=(self_type && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void clear() noexcept
		{
			m_storage.first.clear();
			m_storage.second.clear();
		}

		inline void reserve(size_type const count)
		{
			m_storage.first.reserve(count);
			m_storage.second.reserve(count);
		}

		inline void resize(size_type const count)
		{
			m_storage.first.resize(count);
			m_storage.second.resize(count);
		}

		inline void resize(size_type const count, value_type const & value)
		{
			m_storage.first.resize(count);
			m_storage.second.resize(count, value);
		}

		inline void shrink_to_fit()
		{
			m_storage.first.shrink_to_fit();
			m_storage.second.shrink_to_fit();
		}

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline size_type capacity() const noexcept
		{
			return m_storage.first.capacity();
		}

		ML_NODISCARD inline bool contains(key_type const & key) const
		{
			return m_storage.first.contains(key);
		}

		ML_NODISCARD inline bool empty() const noexcept
		{
			return m_storage.first.empty();
		}

		ML_NODISCARD inline allocator_pair get_allocator() const noexcept
		{
			return { get_key_allocator(), get_value_allocator() };
		}

		ML_NODISCARD inline key_allocator get_key_allocator() const noexcept
		{
			return m_storage.first.get_allocator();
		}

		ML_NODISCARD inline value_allocator get_value_allocator() const noexcept
		{
			return m_storage.second.get_allocator();
		}

		ML_NODISCARD inline size_type max_size() const noexcept
		{
			return m_storage.first.max_size();
		}

		ML_NODISCARD inline size_type size() const noexcept
		{
			return m_storage.first.size();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline value_iterator get(const_key_iterator loc)
		{
			return (loc == m_storage.first.cend())
				? m_storage.second.end()
				: std::next(
					m_storage.second.begin(), std::distance(m_storage.first.cbegin(), loc)
				);
		}

		ML_NODISCARD inline const_value_iterator get(const_key_iterator loc) const
		{
			return (loc == m_storage.first.cend())
				? m_storage.second.cend()
				: std::next(
					m_storage.second.cbegin(), std::distance(m_storage.first.cbegin(), loc)
				);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> inline value_iterator emplace(const_key_iterator loc, Args && ... args)
		{
			return m_storage.second.emplace(this->get(loc), std::forward<Args>(args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator_pair erase(key_iterator loc)
		{
			return {
				m_storage.first.erase(loc),
				m_storage.second.erase(this->get(loc))
			};
		}

		inline iterator_pair erase(key_iterator first, key_iterator last)
		{
			return {
				m_storage.first.erase(first, last),
				m_storage.second.erase(this->get(first), this->get(last))
			};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline std::optional<iterator_pair> find(key_type const & key)
		{
			if (auto const it{ m_storage.first.find(key) }; it != m_storage.first.end())
			{
				return std::make_optional(iterator_pair{ it, this->get(it) });
			}
			else
			{
				return std::nullopt;
			}
		}

		ML_NODISCARD inline std::optional<const_iterator_pair> find(key_type const & key) const
		{
			if (auto const it{ m_storage.first.find(key) }; it != m_storage.first.cend())
			{
				return std::make_optional(const_iterator_pair{ it, this->get(it) });
			}
			else
			{
				return std::nullopt;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> inline self_type & for_each(const_key_iterator first, const_key_iterator last, Fn fn)
		{
			if (!empty())
			{
				for (; first != last; ++first)
				{
					std::invoke(fn, *first, *this->get(first));
				}
			}
			return (*this);
		}

		template <class Fn
		> inline self_type & for_each(const_key_iterator first, Fn fn)
		{
			return this->for_each(first, m_storage.first.end(), fn);
		}

		template <class Fn
		> inline self_type & for_each(Fn fn)
		{
			return this->for_each(m_storage.first.begin(), fn);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> inline self_type const & for_each(const_key_iterator first, const_key_iterator last, Fn fn) const
		{
			if (!empty())
			{
				for (; first != last; ++first)
				{
					std::invoke(fn, *first, *this->get(first));
				}
			}
			return (*this);
		}

		template <class Fn
		> inline self_type const & for_each(const_key_iterator first, Fn fn) const
		{
			return this->for_each(first, m_storage.first.end(), fn);
		}

		template <class Fn
		> inline self_type const & for_each(Fn fn) const
		{
			return this->for_each(m_storage.first.begin(), fn);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> inline self_type & for_each_n(const_key_iterator first, ptrdiff_t count, Fn fn)
		{
			if (!empty() && (0 < count))
			{
				do {
					std::invoke(fn, *first, *this->get(first));
					--count;
					++first;
				} while (0 < count);
			}
			return (*this);
		}

		template <class Fn
		> inline self_type & for_each_n(ptrdiff_t count, Fn fn)
		{
			return this->for_each_n(m_storage.first.begin(), count, fn);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> inline self_type const & for_each_n(const_key_iterator first, ptrdiff_t count, Fn fn) const
		{
			if (!empty() && (0 < count))
			{
				do {
					std::invoke(fn, *first, *get(first));
					--count;
					++first;
				} while (0 < count);
			}
			return (*this);
		}

		template <class Fn
		> inline self_type const & for_each_n(ptrdiff_t count, Fn fn) const
		{
			return this->for_each_n(m_storage.first.begin(), count, fn);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Flat Map
	template <class Key, class Value, class Compare = std::less<Key>, template <class> class Alloc = std::allocator
	> struct flat_map : basic_flat_map<basic_flat_map_traits<Key, Value, Compare, Alloc, false>>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type						= typename flat_map<Key, Value, Compare>;
		using traits_type					= typename basic_flat_map_traits<Key, Value, Compare, Alloc, false>;
		using base_type						= typename basic_flat_map<traits_type>;
		using key_type						= typename base_type::key_type;
		using value_type					= typename base_type::value_type;
		using compare_type					= typename base_type::compare_type;
		using difference_type				= typename base_type::difference_type;
		using size_type						= typename base_type::size_type;
		
		using key_allocator					= typename base_type::key_allocator;
		using key_storage					= typename base_type::key_storage;
		using key_iterator					= typename base_type::key_iterator;
		using const_key_iterator			= typename base_type::const_key_iterator;
		using reverse_key_iterator			= typename base_type::reverse_key_iterator;
		using const_reverse_key_iterator	= typename base_type::const_reverse_key_iterator;
		
		using value_allocator				= typename base_type::value_allocator;
		using value_storage					= typename base_type::value_storage;
		using value_iterator				= typename base_type::value_iterator;
		using const_value_iterator			= typename base_type::const_value_iterator;
		using reverse_value_iterator		= typename base_type::reverse_value_iterator;
		using const_reverse_value_iterator	= typename base_type::const_reverse_value_iterator;
		
		using storage_type					= typename base_type::storage_type;
		using initializer_type				= typename base_type::initializer_type;
		using allocator_pair				= typename base_type::allocator_pair;
		using iterator_pair					= typename base_type::iterator_pair;
		using const_iterator_pair			= typename base_type::const_iterator_pair;
		using reverse_iterator_pair			= typename base_type::reverse_iterator_pair;
		using const_reverse_iterator_pair	= typename base_type::const_reverse_iterator_pair;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		flat_map() noexcept : base_type{} {}

		~flat_map() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		flat_map(initializer_type init)
			: base_type{}
		{
			for (auto it = init.begin(); it != init.end(); ++it)
			{
				this->insert(it->first, it->second);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		flat_map(self_type const & other)
			: base_type{ other.m_storage }
		{
		}

		flat_map(self_type && other) noexcept
			: base_type{ std::move(other.m_storage) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		flat_map(self_type const & other, allocator_pair const & alloc)
			: base_type{ other.m_storage, alloc }
		{
		}

		flat_map(self_type && other, allocator_pair const & alloc) noexcept
			: base_type{ std::move(other.m_storage), alloc }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(initializer_type init)
		{
			self_type temp{ init };
			base_type::swap(temp);
			return (*this);
		}

		inline self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			base_type::swap(temp);
			return (*this);
		}

		inline self_type & operator=(self_type && other) noexcept
		{
			base_type::swap(std::move(other));
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> inline std::pair<iterator_pair, bool> insert(key_type const & key, Args && ... args)
		{
			if (auto const k{ m_storage.first.insert(key) }; k.second)
			{
				return { { k.first, base_type::emplace(k.first, std::forward<Args>(args)...) }, true };
			}
			else
			{
				return { { k.first, base_type::get(k.first) }, false };
			}
		}

		template <class Key, class ... Args
		> inline std::pair<iterator_pair, bool> insert(Key && key, Args && ... args)
		{
			if (auto const k{ m_storage.first.insert(std::move(key)) }; k.second)
			{
				return { { k.first, base_type::emplace(k.first, std::forward<Args>(args)...) }, true };
			}
			else
			{
				return { { k.first, base_type::get(k.first) }, false };
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline value_type & at(key_type const & key)
		{
			if (auto const it{ base_type::find(key) })
			{
				return (*it->second);
			}
			else
			{
				return (*this->insert(key, value_type{}).first.second);
			}
		}

		ML_NODISCARD inline value_type & at(key_type && key)
		{
			if (auto const it{ base_type::find(std::move(key)) })
			{
				return (*it->second);
			}
			else
			{
				return (*this->insert(std::move(key), value_type{}).first.second);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline value_type & operator[](key_type const & key)
		{
			return this->at(key);
		}

		ML_NODISCARD inline value_type & operator[](key_type && key)
		{
			return this->at(std::move(key));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FLAT_MAP_HPP_