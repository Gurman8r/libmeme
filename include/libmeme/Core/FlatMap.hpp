#ifndef _ML_FLAT_MAP_HPP_
#define _ML_FLAT_MAP_HPP_

#include <libmeme/Core/FlatSet.hpp>

namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// FLAT MAP TRAITS
	template <
		class	_Kt,	// key type
		class	_Vt,	// value type
		class	_Pr,	// key comparator predicate type
		size_t	_Th		// search algorithm selector threshold
	> struct flat_map_traits final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using key_type			= typename _Kt;
		using value_type		= typename _Vt;
		using compare_type		= typename _Pr;
		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using difference_type	= typename ptrdiff_t;
		using size_type			= typename size_t;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using key_storage				= typename ds::flat_set<key_type, compare_type, _Th>;
		using key_pointer				= typename key_storage::pointer;
		using const_key_pointer			= typename key_storage::const_pointer;
		using key_reference				= typename key_storage::reference;
		using const_key_reference		= typename key_storage::const_reference;
		using key_iterator				= typename key_storage::iterator;
		using const_key_iterator		= typename key_storage::const_iterator;

		using value_storage				= typename pmr::vector<value_type>;
		using value_pointer				= typename value_storage::pointer;
		using const_value_pointer		= typename value_storage::const_pointer;
		using value_reference			= typename value_storage::reference;
		using const_value_reference		= typename value_storage::const_reference;
		using value_iterator			= typename value_storage::iterator;
		using const_value_iterator		= typename value_storage::const_iterator;

		using storage_type				= typename std::pair<key_storage, value_storage>;
		using keyval_pair				= typename std::pair<key_type, value_type>;
		using init_type					= typename std::initializer_list<keyval_pair>;

		using pointer_pair				= typename std::pair<key_pointer, value_pointer>;
		using const_pointer_pair		= typename std::pair<const_key_pointer, const_value_pointer>;
		using reference_pair			= typename std::pair<key_reference, value_reference>;
		using const_reference_pair		= typename std::pair<const_key_reference, const_value_reference>;

		using iterator_pair				= typename std::pair<key_iterator, value_iterator>;
		using const_iterator_pair		= typename std::pair<const_key_iterator, const_value_iterator>;
		using optl_iterator_pair		= typename std::optional<iterator_pair>;
		using optl_const_iterator_pair	= typename std::optional<const_iterator_pair>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// BASIC FLAT MAP
	template <class _Traits
	> struct basic_flat_map final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using traits_type				= typename _Traits;
		using self_type					= typename basic_flat_map<traits_type>;
		using compare_type				= typename traits_type::compare_type;
		using allocator_type			= typename traits_type::allocator_type;
		using difference_type			= typename traits_type::difference_type;
		using size_type					= typename traits_type::size_type;

		using key_type					= typename traits_type::key_type;
		using key_storage				= typename traits_type::key_storage;
		using key_pointer				= typename traits_type::key_pointer;
		using const_key_pointer			= typename traits_type::const_key_pointer;
		using key_reference				= typename traits_type::key_reference;
		using const_key_reference		= typename traits_type::const_key_reference;
		using key_iterator				= typename traits_type::key_iterator;
		using const_key_iterator		= typename traits_type::const_key_iterator;

		using value_type				= typename traits_type::value_type;
		using value_storage				= typename traits_type::value_storage;
		using value_pointer				= typename traits_type::value_pointer;
		using const_value_pointer		= typename traits_type::const_value_pointer;
		using value_reference			= typename traits_type::value_reference;
		using const_value_reference		= typename traits_type::const_value_reference;
		using value_iterator			= typename traits_type::value_iterator;
		using const_value_iterator		= typename traits_type::const_value_iterator;

		using storage_type				= typename traits_type::storage_type;
		using keyval_pair				= typename traits_type::keyval_pair;
		using init_type					= typename traits_type::init_type;

		using pointer_pair				= typename traits_type::pointer_pair;
		using const_pointer_pair		= typename traits_type::const_pointer_pair;
		using reference_pair			= typename traits_type::reference_pair;
		using const_reference_pair		= typename traits_type::const_reference_pair;

		using iterator_pair				= typename traits_type::iterator_pair;
		using const_iterator_pair		= typename traits_type::const_iterator_pair;
		using optl_iterator_pair		= typename traits_type::optl_iterator_pair;
		using optl_const_iterator_pair	= typename traits_type::optl_const_iterator_pair;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_flat_map(allocator_type const & alloc = {})
			: m_pair{ key_storage{ alloc }, value_storage{ alloc } }
		{
		}

		basic_flat_map(init_type value, allocator_type const & alloc = {})
			: self_type{ alloc }
		{
			this->assign(value);
		}

		basic_flat_map(storage_type const & value, allocator_type const & alloc = {})
			: self_type{ alloc }
		{
			this->assign(value);
		}

		basic_flat_map(storage_type && value, allocator_type const & alloc = {}) noexcept
			: self_type{ alloc }
		{
			this->swap(std::move(value));
		}

		basic_flat_map(self_type const & other, allocator_type const & alloc = {})
			: self_type{ alloc }
		{
			this->assign(other);
		}

		basic_flat_map(self_type && other, allocator_type const & alloc = {}) noexcept
			: self_type{ alloc }
		{
			this->swap(std::move(other));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(init_type value)
		{
			self_type temp{ value };
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

		inline void assign(init_type value)
		{
			if (!this->empty()) { this->clear(); }

			for (auto it = value.begin(); it != value.end(); ++it)
			{
				this->insert(it->first, it->second);
			}
		}

		inline void assign(storage_type const & value)
		{
			if (std::addressof(m_pair) != std::addressof(value))
			{
				m_pair = value;
			}
		}

		inline void assign(self_type const & other)
		{
			if (this != std::addressof(other))
			{
				m_pair = other.m_pair;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_pair.swap(other.m_pair);
			}
		}

		inline void swap(storage_type & value) noexcept
		{
			if (std::addressof(m_pair) != std::addressof(value))
			{
				m_pair.swap(value);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void clear() noexcept
		{
			m_pair.first.clear();
			m_pair.second.clear();
		}

		inline void pop_back() noexcept
		{
			m_pair.first.pop_back();
			m_pair.second.pop_back();
		}

		inline void reserve(size_type const count)
		{
			m_pair.first.reserve(count);
			m_pair.second.reserve(count);
		}

		inline void resize(size_type const count)
		{
			m_pair.first.resize(count);
			m_pair.second.resize(count);
		}

		inline void resize(size_type const count, value_type const & value)
		{
			m_pair.first.resize(count);
			m_pair.second.resize(count, value);
		}

		inline void shrink_to_fit()
		{
			m_pair.first.shrink_to_fit();
			m_pair.second.shrink_to_fit();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline key_storage const & keys() const & noexcept
		{
			return m_pair.first;
		}

		ML_NODISCARD inline value_storage const & values() const & noexcept
		{
			return m_pair.second;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline size_type capacity() const noexcept
		{
			return m_pair.first.capacity();
		}

		ML_NODISCARD inline bool empty() const noexcept
		{
			return m_pair.first.empty();
		}

		ML_NODISCARD inline size_type max_size() const noexcept
		{
			return m_pair.first.max_size();
		}

		ML_NODISCARD inline size_type size() const noexcept
		{
			return m_pair.first.size();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline reference_pair back() noexcept
		{
			return { m_pair.first.back(), m_pair.second.back() };
		}

		ML_NODISCARD inline const_reference_pair back() const noexcept
		{
			return { m_pair.first.back(), m_pair.second.back() };
		}

		ML_NODISCARD inline reference_pair front() noexcept
		{
			return { m_pair.first.front(), m_pair.second.front() };
		}

		ML_NODISCARD inline const_reference_pair front() const noexcept
		{
			return { m_pair.first.front(), m_pair.second.front() };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline value_iterator fetch(const_key_iterator it) noexcept
		{
			return std::next(m_pair.second.begin(), std::distance(m_pair.first.cbegin(), it));
		}

		ML_NODISCARD inline const_value_iterator fetch(const_key_iterator it) const noexcept
		{
			return std::next(m_pair.second.cbegin(), std::distance(m_pair.first.cbegin(), it));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator_pair erase(key_iterator it)
		{
			auto const result{ m_pair.second.erase(this->fetch(it)) };
			return { m_pair.first.erase(it), result };
		}

		inline iterator_pair erase(key_iterator first, key_iterator last)
		{
			auto const result{ m_pair.second.erase(this->fetch(first), this->fetch(last)) };
			return { m_pair.first.erase(first, last), result };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Other = self_type
		> ML_NODISCARD inline auto compare(Other const & other) const
		{
			if constexpr (std::is_same_v<Other, self_type>)
			{
				return this->compare(other.m_pair);
			}
			else
			{
				return (m_pair != other) ? ((m_pair < other) ? -1 : 1) : 0;
			}
		}

		ML_NODISCARD inline bool contains(key_type const & key) const
		{
			return m_pair.first.contains(key);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline optl_iterator_pair find(key_type const & key)
		{
			if (auto const k{ m_pair.first.find(key) }; k != m_pair.first.end())
			{
				return std::make_optional(iterator_pair{ k, this->fetch(k) });
			}
			else
			{
				return std::nullopt;
			}
		}

		ML_NODISCARD inline optl_const_iterator_pair find(key_type const & key) const
		{
			if (auto const k{ m_pair.first.find(key) }; k != m_pair.first.cend())
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
		> inline std::pair<iterator_pair, bool> try_emplace(key_type const & key, Args && ... args)
		{
			if (auto const k{ m_pair.first.insert(key) }; k.second)
			{
				return { this->impl_emplace_hint(k.first, ML_forward(args)...), true };
			}
			else
			{
				return { { k.first, this->fetch(k.first) }, false };
			}
		}

		template <class ... Args
		> inline std::pair<iterator_pair, bool> try_emplace(key_type && key, Args && ... args)
		{
			if (auto const k{ m_pair.first.insert(std::move(key)) }; k.second)
			{
				return { this->impl_emplace_hint(k.first, ML_forward(args)...), true };
			}
			else
			{
				return { { k.first, this->fetch(k.first) }, false };
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Key, class Value
		> inline iterator_pair insert(Key && key, Value && value)
		{
			return this->try_emplace(ML_forward(key), ML_forward(value)).first;
		}

		inline iterator_pair insert(keyval_pair && pair)
		{
			return this->try_emplace(ML_forward(pair.first), ML_forward(pair.second)).first;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline value_type & find_or_add(key_type const & key, value_type && value)
		{
			if (auto const it{ this->find(key) })
			{
				return (*it->second);
			}
			else
			{
				return (*this->insert(key, ML_forward(value)).second);
			}
		}

		ML_NODISCARD inline value_type & find_or_add(key_type && key, value_type && value)
		{
			if (auto const it{ this->find(key) })
			{
				return (*it->second);
			}
			else
			{
				return (*this->insert(ML_forward(key), ML_forward(value)).second);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline value_type & at(key_type const & key)
		{
			return this->find_or_add(key, value_type{});
		}

		ML_NODISCARD inline value_type & at(key_type && key)
		{
			return this->find_or_add(ML_forward(key), value_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline value_type & operator[](key_type const & key) noexcept
		{
			return this->at(key);
		}

		ML_NODISCARD inline value_type & operator[](key_type && key) noexcept
		{
			return this->at(ML_forward(key));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> inline auto for_each(const_key_iterator first, const_key_iterator last, Fn fn)
		{
			for (; first != last; ++first)
			{
				std::invoke(fn, *first, *this->fetch(first));
			}
			return first;
		}

		template <class Fn
		> inline auto for_each(const_key_iterator first, Fn fn)
		{
			return this->for_each(first, m_pair.first.cend(), fn);
		}

		template <class Fn
		> inline auto for_each(Fn fn)
		{
			return this->for_each(m_pair.first.cbegin(), fn);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> inline auto for_each(const_key_iterator first, const_key_iterator last, Fn fn) const
		{
			for (; first != last; ++first)
			{
				std::invoke(fn, *first, *this->fetch(first));
			}
			return first;
		}

		template <class Fn
		> inline auto for_each(const_key_iterator first, Fn fn) const
		{
			return this->for_each(first, m_pair.first.cend(), fn);
		}

		template <class Fn
		> inline auto for_each(Fn fn) const
		{
			return this->for_each(m_pair.first.cbegin(), fn);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> inline auto for_each_n(const_key_iterator first, ptrdiff_t count, Fn fn)
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
		> inline auto for_each_n(ptrdiff_t count, Fn fn)
		{
			return this->for_each_n(m_pair.first.cbegin(), count, fn);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> inline auto for_each_n(const_key_iterator first, ptrdiff_t count, Fn fn) const
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
		> inline auto for_each_n(ptrdiff_t count, Fn fn) const
		{
			return this->for_each_n(m_pair.first.cbegin(), count, fn);
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
		storage_type m_pair;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// emplace_hint implementation
		template <class ... Args
		> inline iterator_pair impl_emplace_hint(const_key_iterator it, Args && ... args)
		{
			// must be private or the map could become unsorted
			return {
				std::next(m_pair.first.begin(), std::distance(m_pair.first.cbegin(), it)),
				m_pair.second.emplace(this->fetch(it), ML_forward(args)...)
			};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// FLAT MAP | flat ordered associative container
	template <
		class	_Kt,					// key type
		class	_Vt,					// value type
		class	_Pr = std::less<_Kt>,	// key comparator predicate type
		size_t	_Th = 42				// search algorithm selector threshold
	> ML_ALIAS flat_map = typename basic_flat_map
	<
		flat_map_traits<_Kt, _Vt, _Pr, _Th>
	>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FLAT_MAP_HPP_