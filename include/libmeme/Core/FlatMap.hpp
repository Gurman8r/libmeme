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
		size_t	_Th		// threshold for selecting search algorithm
	> struct flat_map_traits final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using key_type			= typename _Kty;
		using value_type		= typename _Vty;
		using compare_type		= typename _Pr;
		using difference_type	= typename ptrdiff_t;
		using size_type			= typename size_t;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr size_type thresh{ _Th };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using key_storage					= typename ds::flat_set<key_type, compare_type, thresh>;
		using key_iterator					= typename key_storage::iterator;
		using const_key_iterator			= typename key_storage::const_iterator;
		using reverse_key_iterator			= typename key_storage::reverse_iterator;
		using const_reverse_key_iterator	= typename key_storage::const_reverse_iterator;

		using value_storage					= typename pmr::vector<value_type>;
		using value_iterator				= typename value_storage::iterator;
		using const_value_iterator			= typename value_storage::const_iterator;
		using reverse_value_iterator		= typename value_storage::reverse_iterator;
		using const_reverse_value_iterator	= typename value_storage::const_reverse_iterator;
		
		using storage_type					= typename std::pair<key_storage, value_storage>;
		using keyval_pair					= typename std::pair<key_type, value_type>;
		using iterator_pair					= typename std::pair<key_iterator, value_iterator>;
		using const_iterator_pair			= typename std::pair<const_key_iterator, const_value_iterator>;
		using reverse_iterator_pair			= typename std::pair<reverse_key_iterator, reverse_value_iterator>;
		using const_reverse_iterator_pair	= typename std::pair<const_reverse_key_iterator, const_reverse_value_iterator>;
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// BASIC FLAT MAP
	template <class _Traits
	> struct basic_flat_map final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using traits_type					= typename _Traits;
		using self_type						= typename basic_flat_map<traits_type>;
		using compare_type					= typename traits_type::compare_type;
		using allocator_type				= typename traits_type::allocator_type;
		using difference_type				= typename traits_type::difference_type;
		using size_type						= typename traits_type::size_type;
		
		using key_type						= typename traits_type::key_type;
		using key_storage					= typename traits_type::key_storage;
		using key_iterator					= typename traits_type::key_iterator;
		using const_key_iterator			= typename traits_type::const_key_iterator;
		using reverse_key_iterator			= typename traits_type::reverse_key_iterator;
		using const_reverse_key_iterator	= typename traits_type::const_reverse_key_iterator;
		
		using value_type					= typename traits_type::value_type;
		using value_storage					= typename traits_type::value_storage;
		using value_iterator				= typename traits_type::value_iterator;
		using const_value_iterator			= typename traits_type::const_value_iterator;
		using reverse_value_iterator		= typename traits_type::reverse_value_iterator;
		using const_reverse_value_iterator	= typename traits_type::const_reverse_value_iterator;
		
		using storage_type					= typename traits_type::storage_type;
		using keyval_pair					= typename traits_type::keyval_pair;
		using iterator_pair					= typename traits_type::iterator_pair;
		using const_iterator_pair			= typename traits_type::const_iterator_pair;
		using reverse_iterator_pair			= typename traits_type::reverse_iterator_pair;
		using const_reverse_iterator_pair	= typename traits_type::const_reverse_iterator_pair;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_flat_map() noexcept
			: m_storage{}
		{
		}

		explicit basic_flat_map(allocator_type const & alloc) : m_storage{
			key_storage{ alloc },
			value_storage{ alloc }
		}
		{
		}

		basic_flat_map(std::initializer_list<keyval_pair> init, allocator_type const & alloc = {})
			: m_storage{ alloc }
		{
			for (auto pair = init.begin(); pair != init.end(); ++pair)
			{
				this->insert(pair->first, pair->second);
			}
		}

		basic_flat_map(storage_type const & value, allocator_type const & alloc = {}) : m_storage{
			key_storage{ value.first, alloc },
			value_storage{ value.second, alloc }
		}
		{
		}

		basic_flat_map(storage_type && value, allocator_type const & alloc = {}) noexcept : m_storage{
			key_storage{ std::move(value.first), alloc },
			value_storage{ std::move(value.second), alloc }
		}
		{
		}

		basic_flat_map(self_type const & other, allocator_type const & alloc = {})
			: self_type{ other.m_storage, alloc }
		{
		}

		basic_flat_map(self_type && other, allocator_type const & alloc = {}) noexcept
			: self_type{ std::move(other.m_storage), alloc }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(std::initializer_list<keyval_pair> init)
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

		ML_NODISCARD inline bool valid() const noexcept
		{
			return this->size() == m_storage.second.size();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline value_iterator fetch(const_key_iterator loc)
		{
			return std::next(m_storage.second.begin(), std::distance(m_storage.first.cbegin(), loc));
		}

		ML_NODISCARD inline const_value_iterator fetch(const_key_iterator loc) const
		{
			return std::next(m_storage.second.cbegin(), std::distance(m_storage.first.cbegin(), loc));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator_pair erase(key_iterator loc)
		{
			auto const result
			{
				m_storage.second.erase(this->fetch(loc))
			};
			return { m_storage.first.erase(loc), result };
		}

		inline iterator_pair erase(key_iterator first, key_iterator last)
		{
			auto const result
			{
				m_storage.second.erase(this->fetch(first), this->fetch(last))
			};
			return { m_storage.first.erase(first, last), result };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Other = self_type
		> ML_NODISCARD inline auto compare(Other const & other) const
		{
			if constexpr (std::is_same_v<Other, self_type>)
			{
				return this->compare(other.m_storage);
			}
			else
			{
				return (m_storage != other) ? ((m_storage < other) ? -1 : 1) : 0;
			}
		}

		template <class Other = key_type
		> ML_NODISCARD inline bool contains(Other const & key) const
		{
			return m_storage.first.contains(key);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Other = key_type
		> ML_NODISCARD inline std::optional<iterator_pair> find(Other const & other)
		{
			if (auto const k{ m_storage.first.find(other) }; k != m_storage.first.end())
			{
				return std::make_optional(iterator_pair{ k, this->fetch(k) });
			}
			else
			{
				return std::nullopt;
			}
		}

		template <class Other = key_type
		> ML_NODISCARD inline std::optional<const_iterator_pair> find(Other const & other) const
		{
			if (auto const k{ m_storage.first.find(other) }; k != m_storage.first.cend())
			{
				return std::make_optional(const_iterator_pair{ k, this->fetch(k) });
			}
			else
			{
				return std::nullopt;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> inline value_iterator emplace(Args && ... args)
		{
			return m_storage.second.emplace(ML_FWD(args)...);
		}

		template <class ... Args
		> inline iterator_pair emplace_hint(const_key_iterator loc, Args && ... args)
		{
			return { 
				std::next(m_storage.first.begin(), std::distance(m_storage.first.cbegin(), loc)),
				this->emplace(this->fetch(loc), ML_FWD(args)...)
			};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> inline std::pair<iterator_pair, bool> try_emplace(key_type const & key, Args && ... args)
		{
			if (auto const k{ m_storage.first.insert(key) }; k.second)
			{
				return { this->emplace_hint(k.first, ML_FWD(args)...), true };
			}
			else
			{
				return { { k.first, this->fetch(k.first) }, false };
			}
		}

		template <class ... Args
		> inline std::pair<iterator_pair, bool> try_emplace(key_type && key, Args && ... args)
		{
			if (auto const k{ m_storage.first.insert(std::move(key)) }; k.second)
			{
				return { this->emplace_hint(k.first, ML_FWD(args)...), true };
			}
			else
			{
				return { { k.first, this->fetch(k.first) }, false };
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Value
		> inline iterator_pair insert(const_key_iterator loc, Value && value)
		{
			return this->emplace_hint(loc, ML_FWD(value));
		}

		template <class Key, class Value
		> inline iterator_pair insert(Key && key, Value && value)
		{
			return this->try_emplace(ML_FWD(key), ML_FWD(value)).first;
		}

		inline iterator_pair insert(keyval_pair const & pair)
		{
			return this->try_emplace(pair.first, pair.second).first;
		}

		inline iterator_pair insert(keyval_pair && pair)
		{
			return this->try_emplace(std::move(pair.first), std::move(pair.second)).first;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline value_type & at(key_type const & key)
		{
			if (auto const pair{ this->find(key) })
			{
				return (*pair->second);
			}
			else
			{
				return (*this->insert(key, value_type{}).second);
			}
		}

		ML_NODISCARD inline value_type & at(key_type && key)
		{
			if (auto const pair{ this->find(key) })
			{
				return (*pair->second);
			}
			else
			{
				return (*this->insert(std::move(key), value_type{}).second);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline value_type & operator[](key_type const & key) noexcept
		{
			return this->at(key);
		}

		ML_NODISCARD inline value_type & operator[](key_type && key) noexcept
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

		template <class Other = self_type
		> ML_NODISCARD inline bool operator==(Other const & other) const
		{
			return this->compare(other) == 0;
		}

		template <class Other = self_type
		> ML_NODISCARD inline bool operator!=(Other const & other) const
		{
			return this->compare(other) != 0;
		}

		template <class Other = self_type
		> ML_NODISCARD inline bool operator<(Other const & other) const
		{
			return this->compare(other) < 0;
		}

		template <class Other = self_type
		> ML_NODISCARD inline bool operator>(Other const & other) const
		{
			return this->compare(other) > 0;
		}

		template <class Other = self_type
		> ML_NODISCARD inline bool operator<=(Other const & other) const
		{
			return this->compare(other) <= 0;
		}

		template <class Other = self_type
		> ML_NODISCARD inline bool operator>=(Other const & other) const
		{
			return this->compare(other) >= 0;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// FLAT MAP | linear associative container with unique keys
	template <
		class	_Kty,
		class	_Vty,
		class	_Pr = std::less<_Kty>,
		size_t	_Th = 42
	> ML_USING flat_map = typename basic_flat_map<
		flat_map_traits<_Kty, _Vty, _Pr, _Th>
	>;

	// FLAT MULTIMAP | linear associative container that allows duplicate keys (map of sets)
	template <
		class	_Kty,
		class	_Vty,
		class	_Kpr = std::less<_Kty>,
		class	_Vpr = std::less<_Vty>,
		size_t	_Th = 42
	> ML_USING flat_multimap = typename flat_map<
		_Kty,
		ds::flat_set<_Vty, _Vpr, _Th>,
		_Kpr,
		_Th
	>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FLAT_MAP_HPP_