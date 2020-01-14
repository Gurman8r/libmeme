#ifndef _ML_FLAT_MAP_HPP_
#define _ML_FLAT_MAP_HPP_

#include <libmeme/Core/FlatSet.hpp>

// Flat Map
namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Key, class Value, class Compare
	> struct basic_flat_map
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type						= typename basic_flat_map<Key, Value, Compare>;
		using key_type						= typename Key;
		using value_type					= typename Value;
		using compare_type					= typename Compare;
		
		using key_storage					= typename ds::flat_set<key_type, compare_type>;
		using key_iterator					= typename key_storage::iterator;
		using const_key_iterator			= typename key_storage::const_iterator;
		using reverse_key_iterator			= typename key_storage::reverse_iterator;
		using const_reverse_key_iterator	= typename key_storage::const_reverse_iterator;
		
		using value_storage					= typename std::vector<value_type>;
		using value_iterator				= typename value_storage::iterator;
		using const_value_iterator			= typename value_storage::const_iterator;
		using reverse_value_iterator		= typename value_storage::reverse_iterator;
		using const_reverse_value_iterator	= typename value_storage::const_reverse_iterator;
		
		using storage_type					= typename std::pair<key_storage, value_storage>;
		using pair_type						= typename std::pair<key_type, value_type>;
		using initializer_type				= typename std::initializer_list<pair_type>;

		using iterator_pair					= typename std::pair<key_iterator, value_iterator>;
		using const_iterator_pair			= typename std::pair<const_key_iterator, const_value_iterator>;
		using reverse_iterator_pair			= typename std::pair<reverse_key_iterator, reverse_value_iterator>;
		using const_reverse_iterator_pair	= typename std::pair<const_reverse_key_iterator, const_reverse_value_iterator>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_flat_map() noexcept
			: m_storage{}
		{
		}

		basic_flat_map(initializer_type init)
			: m_storage{}
		{
			for (auto it = init.begin(); it != init.end(); ++it)
			{
				try_emplace(it->first, it->second);
			}
		}

		basic_flat_map(self_type const & other)
			: m_storage{ other.m_storage }
		{
		}

		basic_flat_map(self_type && other) noexcept
			: m_storage{ std::move(other.m_storage) }
		{
		}

		~basic_flat_map() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(initializer_type init)
		{
			self_type temp{ init };
			swap(temp);
			return (*this);
		}

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

		inline auto storage() const noexcept ->storage_type const & { return m_storage; }

		inline auto keys() const noexcept -> key_storage const & { return m_storage.first; }

		inline auto values() const noexcept -> value_storage const & { return m_storage.second; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline size_t capacity() const noexcept
		{
			return m_storage.first.capacity();
		}

		inline void clear() noexcept
		{
			m_storage.first.clear();
			m_storage.second.clear();
		}

		inline bool empty() const noexcept
		{
			return m_storage.first.empty();
		}

		inline void reserve(size_t const count)
		{
			m_storage.first.reserve(count);
			m_storage.second.reserve(count);
		}

		inline void resize(size_t const count)
		{
			m_storage.first.resize(count);
			m_storage.second.resize(count);
		}

		inline void resize(size_t const count, value_type const & value)
		{
			m_storage.first.resize(count);
			m_storage.second.resize(count, value);
		}

		inline void shrink_to_fit()
		{
			m_storage.first.shrink_to_fit();
			m_storage.second.shrink_to_fit();
		}

		inline size_t size() const noexcept
		{
			return m_storage.first.size();
		}

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline bool contains(U && key) const
		{
			return m_storage.first.contains(std::forward<U>(key));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline key_iterator get_key(const_value_iterator value)
		{
			return (value == m_storage.second.end())
				? m_storage.first.end()
				: std::next(
					m_storage.first.begin(), std::distance(m_storage.second.cbegin(), value)
				);
		}

		inline const_key_iterator get_key(const_value_iterator value) const
		{
			return (value == m_storage.second.end())
				? m_storage.first.cend()
				: std::next(
					m_storage.first.cbegin(), std::distance(m_storage.second.cbegin(), value)
				);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline value_iterator get_value(const_key_iterator key)
		{
			return (key == m_storage.first.end())
				? m_storage.second.end()
				: std::next(
					m_storage.second.begin(), std::distance(m_storage.first.cbegin(), key)
				);
		}

		inline const_value_iterator get_value(const_key_iterator key) const
		{
			return (key == m_storage.first.end())
				? m_storage.second.cend()
				: std::next(
					m_storage.second.cbegin(), std::distance(m_storage.first.cbegin(), key)
				);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator_pair erase(key_iterator location)
		{
			return {
				m_storage.first.erase(location),
				m_storage.second.erase(this->get_value(location))
			};
		}

		inline iterator_pair erase(key_iterator first, key_iterator last)
		{
			return {
				m_storage.first.erase(first, last),
				m_storage.second.erase(this->get_value(first), this->get_value(last))
			};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::optional<value_iterator> find(key_type const & key)
		{
			if (auto const it{ m_storage.first.find(key) }; it != m_storage.first.end())
			{
				return std::make_optional(this->get_value(it));
			}
			else
			{
				return std::nullopt;
			}
		}

		inline std::optional<const_value_iterator> find(key_type const & key) const
		{
			if (auto const it{ m_storage.first.find(key) }; it != m_storage.first.end())
			{
				return std::make_optional(this->get_value(it));
			}
			else
			{
				return std::nullopt;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> inline value_iterator emplace(const_key_iterator location, Args && ... args)
		{
			return m_storage.second.emplace(this->get_value(location), std::forward<Args>(args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> inline std::pair<iterator_pair, bool> try_emplace(key_type const & key, Args && ... args)
		{
			if (auto const p{ m_storage.first.insert(key) }; p.second)
			{
				return { { p.first, this->emplace(p.first, std::forward<Args>(args)...) }, true };
			}
			else
			{
				return { { p.first, this->get_value(p.first) }, false };
			}
		}

		template <class Key, class ... Args
		> inline std::pair<iterator_pair, bool> try_emplace(Key && key, Args && ... args)
		{
			if (auto const p{ m_storage.first.insert(std::move(key)) }; p.second)
			{
				return { { p.first, this->emplace(p.first, std::forward<Args>(args)...) }, true };
			}
			else
			{
				return { { p.first, this->get_value(p.first) }, false };
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline value_type & at(key_type const & key)
		{
			if (auto const it{ find(key) })
			{
				return (**it);
			}
			else
			{
				return (*this->try_emplace(key, value_type{}).first.second);
			}
		}

		inline value_type & operator[](key_type const & key)
		{
			return at(key);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> inline self_type & for_each(key_iterator first, key_iterator last, Fn fn)
		{
			if (!empty())
			{
				for (; first != last; ++first)
				{
					std::invoke(fn, *first, *get_value(first));
				}
			}
			return (*this);
		}

		template <class Fn
		> inline self_type & for_each(key_iterator first, Fn fn)
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
					std::invoke(fn, *first, *get_value(first));
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
		> inline self_type & for_each_n(key_iterator first, ptrdiff_t count, Fn fn)
		{
			if (!empty() && (0 < count))
			{
				do {
					std::invoke(fn, *first, *get_value(first));
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
					std::invoke(fn, *first, *get_value(first));
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

	template <class Key, class Value
	> ML_USING flat_map = typename basic_flat_map<
		Key, Value,
		std::less<Key>
	>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FLAT_MAP_HPP_