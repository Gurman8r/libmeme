#ifndef _ML_FLAT_MAP_HPP_
#define _ML_FLAT_MAP_HPP_

#include <libmeme/Core/FlatSet.hpp>

namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// BASIC FLAT MAP TRAITS
	template <class _Kty,	// key type
		class _Vty,			// value type
		class _Pr,			// key comparator predicate type
		class _Kal,			// key allocator type
		class _Val,			// value allocator type
		bool _Multi			// true if multiple equivalent keys are permitted (NYI)
	> struct basic_flat_map_traits
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using key_type = typename _Kty;
		
		using value_type = typename _Vty;
		
		using compare_type = typename _Pr;

		using key_allocator = typename _Kal;

		using value_allocator = typename _Val;

		static constexpr bool multi{ _Multi };

		using difference_type = typename ptrdiff_t;
		
		using size_type = typename size_t;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// BASIC FLAT MAP
	template <class _Traits
	> struct basic_flat_map
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using traits_type					= typename _Traits;
		using self_type						= typename basic_flat_map<traits_type>;
		using compare_type					= typename traits_type::compare_type;
		using difference_type				= typename traits_type::difference_type;
		using size_type						= typename traits_type::size_type;
		
		using key_type						= typename traits_type::key_type;
		using key_allocator					= typename traits_type::key_allocator;
		using key_storage					= typename ds::flat_set<key_type, compare_type, key_allocator>;
		using key_iterator					= typename key_storage::iterator;
		using const_key_iterator			= typename key_storage::const_iterator;
		using reverse_key_iterator			= typename key_storage::reverse_iterator;
		using const_reverse_key_iterator	= typename key_storage::const_reverse_iterator;
		
		using value_type					= typename traits_type::value_type;
		using value_allocator				= typename traits_type::value_allocator;
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

		explicit basic_flat_map(storage_type const & value, allocator_pair const & alloc) : self_type{
			key_storage{ value.first, alloc.first },
			value_storage{ value.second, alloc.second }
		}
		{
		}

		explicit basic_flat_map(storage_type && value, allocator_pair const & alloc) noexcept : self_type{
			key_storage{ std::move(value.first), alloc.first },
			value_storage{ std::move(value.second), alloc.second }
		}
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

		ML_NODISCARD inline allocator_pair get_allocators() const noexcept
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

		ML_NODISCARD inline value_iterator fetch(const_key_iterator addr)
		{
			return (addr == m_storage.first.cend())
				? m_storage.second.end()
				: std::next(
					m_storage.second.begin(), std::distance(m_storage.first.cbegin(), addr)
				);
		}

		ML_NODISCARD inline const_value_iterator fetch(const_key_iterator addr) const
		{
			return (addr == m_storage.first.cend())
				? m_storage.second.cend()
				: std::next(
					m_storage.second.cbegin(), std::distance(m_storage.first.cbegin(), addr)
				);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> inline value_iterator emplace(const_key_iterator addr, Args && ... args)
		{
			return m_storage.second.emplace(this->fetch(addr), std::forward<Args>(args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator_pair erase(key_iterator addr)
		{
			auto const temp{ m_storage.second.erase(this->fetch(addr)) };
			return { m_storage.first.erase(addr), temp };
		}

		inline iterator_pair erase(key_iterator first, key_iterator last)
		{
			auto const temp{ m_storage.second.erase(this->fetch(first), this->fetch(last)) };
			return { m_storage.first.erase(first, last), temp };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline std::optional<iterator_pair> find(key_type const & key)
		{
			if (auto const it{ m_storage.first.find(key) }; it != m_storage.first.end())
			{
				return std::make_optional(iterator_pair{ it, this->fetch(it) });
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
				return std::make_optional(const_iterator_pair{ it, this->fetch(it) });
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
					std::invoke(fn, *first, *this->fetch(first));
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
		> inline const_key_iterator for_each(const_key_iterator first, const_key_iterator last, Fn fn) const
		{
			if (!empty())
			{
				for (; first != last; ++first)
				{
					std::invoke(fn, *first, *this->fetch(first));
				}
			}
			return first;
		}

		template <class Fn
		> inline const_key_iterator for_each(const_key_iterator first, Fn fn) const
		{
			return this->for_each(first, m_storage.first.end(), fn);
		}

		template <class Fn
		> inline const_key_iterator for_each(Fn fn) const
		{
			return this->for_each(m_storage.first.begin(), fn);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> inline const_key_iterator for_each_n(const_key_iterator first, ptrdiff_t count, Fn fn)
		{
			if (!empty() && (0 < count))
			{
				do {
					std::invoke(fn, *first, *this->fetch(first));
					--count;
					++first;
				} while (0 < count);
			}
			return first;
		}

		template <class Fn
		> inline const_key_iterator for_each_n(ptrdiff_t count, Fn fn)
		{
			return this->for_each_n(m_storage.first.begin(), count, fn);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> inline const_key_iterator for_each_n(const_key_iterator first, ptrdiff_t count, Fn fn) const
		{
			if (!empty() && (0 < count))
			{
				do {
					std::invoke(fn, *first, *fetch(first));
					--count;
					++first;
				} while (0 < count);
			}
			return first;
		}

		template <class Fn
		> inline const_key_iterator for_each_n(ptrdiff_t count, Fn fn) const
		{
			return this->for_each_n(m_storage.first.begin(), count, fn);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool operator==(self_type const & other) const
		{
			return (m_storage == other.m_storage);
		}

		ML_NODISCARD inline bool operator!=(self_type const & other) const
		{
			return (m_storage != other.m_storage);
		}

		ML_NODISCARD inline bool operator<(self_type const & other) const
		{
			return (m_storage < other.m_storage);
		}

		ML_NODISCARD inline bool operator>(self_type const & other) const
		{
			return (m_storage > other.m_storage);
		}

		ML_NODISCARD inline bool operator<=(self_type const & other) const
		{
			return (m_storage <= other.m_storage);
		}

		ML_NODISCARD inline bool operator>=(self_type const & other) const
		{
			return (m_storage >= other.m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// FLAT MAP
	template <class _Kty, class _Vty,
		class _Pr	= std::less<_Kty>,
		class _Kal	= std::allocator<_Kty>,
		class _Val	= std::allocator<_Vty>
	> struct flat_map : basic_flat_map<
		basic_flat_map_traits<_Kty, _Vty, _Pr, _Kal, _Val, false>
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type						= typename flat_map<_Kty, _Vty, _Pr>;
		using base_type						= typename basic_flat_map<basic_flat_map_traits<_Kty, _Vty, _Pr, _Kal, _Val, false>>;
		using traits_type					= typename base_type::traits_type;
		using compare_type					= typename base_type::compare_type;
		using difference_type				= typename base_type::difference_type;
		using size_type						= typename base_type::size_type;
		
		using key_type						= typename base_type::key_type;
		using key_allocator					= typename base_type::key_allocator;
		using key_storage					= typename base_type::key_storage;
		using key_iterator					= typename base_type::key_iterator;
		using const_key_iterator			= typename base_type::const_key_iterator;
		using reverse_key_iterator			= typename base_type::reverse_key_iterator;
		using const_reverse_key_iterator	= typename base_type::const_reverse_key_iterator;
		
		using value_type					= typename base_type::value_type;
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
				return { { k.first, base_type::fetch(k.first) }, false };
			}
		}

		template <class ... Args
		> inline std::pair<iterator_pair, bool> insert(key_type && key, Args && ... args)
		{
			if (auto const k{ m_storage.first.insert(std::move(key)) }; k.second)
			{
				return { { k.first, base_type::emplace(k.first, std::forward<Args>(args)...) }, true };
			}
			else
			{
				return { { k.first, base_type::fetch(k.first) }, false };
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