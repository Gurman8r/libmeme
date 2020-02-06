#ifndef _ML_FLAT_MAP_HPP_
#define _ML_FLAT_MAP_HPP_

#include <libmeme/Core/FlatSet.hpp>

namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// FLAT MAP TRAITS
	template <
		class	_Kty,	// key type
		class	_Vty,	// value type
		class	_Pr,	// key comparator predicate type
		bool	_Multi,	// true if multiple equivalent keys are permitted
		size_t	_Thresh	// size threshold used to determine search algorithm
	> struct flat_map_traits
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using key_type = typename _Kty;
		
		using mapped_type = typename _Vty;
		
		using compare_type = typename _Pr;

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		static constexpr bool multi{ _Multi };

		static constexpr size_t thresh{ _Thresh };

		using difference_type = typename ptrdiff_t;
		
		using size_type = typename size_t;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// FLAT MAP
	template <class _Kty, class _Vty, class _Pr = std::less<_Kty>, size_t _Thresh = 42
	> struct flat_map final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type						= typename flat_map<_Kty, _Vty, _Pr, _Thresh>;
		using traits_type					= typename flat_map_traits<_Kty, _Vty, _Pr, false, _Thresh>;
		using compare_type					= typename traits_type::compare_type;
		using allocator_type				= typename traits_type::allocator_type;
		using difference_type				= typename traits_type::difference_type;
		using size_type						= typename traits_type::size_type;
		
		using key_type						= typename traits_type::key_type;
		using key_storage					= typename ds::flat_set<key_type, compare_type, traits_type::thresh>;
		using key_iterator					= typename key_storage::iterator;
		using const_key_iterator			= typename key_storage::const_iterator;
		using reverse_key_iterator			= typename key_storage::reverse_iterator;
		using const_reverse_key_iterator	= typename key_storage::const_reverse_iterator;
		
		using mapped_type					= typename traits_type::mapped_type;
		using mapped_storage				= typename pmr::vector<mapped_type>;
		using mapped_iterator				= typename mapped_storage::iterator;
		using const_mapped_iterator			= typename mapped_storage::const_iterator;
		using reverse_mapped_iterator		= typename mapped_storage::reverse_iterator;
		using const_reverse_mapped_iterator	= typename mapped_storage::const_reverse_iterator;
		
		using storage_type					= typename std::pair<key_storage, mapped_storage>;
		using pair_type						= typename std::pair<key_type, mapped_type>;
		using initializer_type				= typename std::initializer_list<pair_type>;
		using iterator_pair					= typename std::pair<key_iterator, mapped_iterator>;
		using const_iterator_pair			= typename std::pair<const_key_iterator, const_mapped_iterator>;
		using reverse_iterator_pair			= typename std::pair<reverse_key_iterator, reverse_mapped_iterator>;
		using const_reverse_iterator_pair	= typename std::pair<const_reverse_key_iterator, const_reverse_mapped_iterator>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		flat_map() noexcept
			: m_storage{}
		{
		}

		explicit flat_map(allocator_type const & alloc) : m_storage{
			key_storage{ alloc },
			mapped_storage{ alloc }
		}
		{
		}

		flat_map(initializer_type init, allocator_type const & alloc = {})
			: m_storage{ alloc }
		{
			for (auto it = init.begin(); it != init.end(); ++it)
			{
				this->insert(it->first, it->second);
			}
		}

		flat_map(storage_type const & value, allocator_type const & alloc = {}) : m_storage{
			key_storage{ value.first, alloc },
			mapped_storage{ value.second, alloc }
		}
		{
		}

		flat_map(storage_type && value, allocator_type const & alloc = {}) noexcept : m_storage{
			key_storage{ std::move(value.first), alloc },
			mapped_storage{ std::move(value.second), alloc }
		}
		{
		}

		flat_map(self_type const & other, allocator_type const & alloc = {})
			: self_type{ other.m_storage, alloc }
		{
		}

		flat_map(self_type && other, allocator_type const & alloc = {}) noexcept
			: self_type{ std::move(other.m_storage), alloc }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(initializer_type init)
		{
			self_type temp{ init };
			this->swap(temp);
			return (*this);
		}

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

		inline void resize(size_type const count, mapped_type const & value)
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

		ML_NODISCARD inline size_type max_size() const noexcept
		{
			return m_storage.first.max_size();
		}

		ML_NODISCARD inline size_type size() const noexcept
		{
			return m_storage.first.size();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline key_storage const & get_keys() const noexcept
		{
			return m_storage.first;
		}

		ML_NODISCARD inline mapped_storage const & get_values() const noexcept
		{
			return m_storage.second;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline mapped_iterator fetch(const_key_iterator addr)
		{
			return (addr == m_storage.first.cend())
				? m_storage.second.end()
				: std::next(
					m_storage.second.begin(), std::distance(m_storage.first.cbegin(), addr)
				);
		}

		ML_NODISCARD inline const_mapped_iterator fetch(const_key_iterator addr) const
		{
			return (addr == m_storage.first.cend())
				? m_storage.second.cend()
				: std::next(
					m_storage.second.cbegin(), std::distance(m_storage.first.cbegin(), addr)
				);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> inline mapped_iterator emplace(const_key_iterator addr, Args && ... args)
		{
			return m_storage.second.emplace(this->fetch(addr), ML_FWD(args)...);
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

		template <class ... Args
		> inline std::pair<iterator_pair, bool> insert(key_type const & key, Args && ... args)
		{
			if (auto const k{ m_storage.first.insert(key) }; k.second)
			{
				return { { k.first, this->emplace(k.first, ML_FWD(args)...) }, true };
			}
			else
			{
				return { { k.first, this->fetch(k.first) }, false };
			}
		}

		template <class ... Args
		> inline std::pair<iterator_pair, bool> insert(key_type && key, Args && ... args)
		{
			if (auto const k{ m_storage.first.insert(std::move(key)) }; k.second)
			{
				return { { k.first, this->emplace(k.first, ML_FWD(args)...) }, true };
			}
			else
			{
				return { { k.first, this->fetch(k.first) }, false };
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline mapped_type & at(key_type const & key)
		{
			if (auto const it{ this->find(key) })
			{
				return (*it->second);
			}
			else
			{
				return (*this->insert(key, mapped_type{}).first.second);
			}
		}

		ML_NODISCARD inline mapped_type & at(key_type && key)
		{
			if (auto const it{ this->find(key) })
			{
				return (*it->second);
			}
			else
			{
				return (*this->insert(std::move(key), mapped_type{}).first.second);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline mapped_type & operator[](key_type const & key)
		{
			return this->at(key);
		}

		ML_NODISCARD inline mapped_type & operator[](key_type && key)
		{
			return this->at(std::move(key));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> inline const_key_iterator for_each(const_key_iterator first, const_key_iterator last, Fn fn)
		{
			for (; first != last; ++first)
			{
				std::invoke(fn, *first, *this->fetch(first));
			}
			return first;
		}

		template <class Fn
		> inline const_key_iterator for_each(const_key_iterator first, Fn fn)
		{
			return this->for_each(first, m_storage.first.end(), fn);
		}

		template <class Fn
		> inline const_key_iterator for_each(Fn fn)
		{
			return this->for_each(m_storage.first.begin(), fn);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> inline const_key_iterator for_each(const_key_iterator first, const_key_iterator last, Fn fn) const
		{
			for (; first != last; ++first)
			{
				std::invoke(fn, *first, *this->fetch(first));
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
			if (0 < count)
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
			if (0 < count)
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

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FLAT_MAP_HPP_