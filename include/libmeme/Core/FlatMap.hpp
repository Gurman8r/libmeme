#ifndef _ML_FLAT_MAP_HPP_
#define _ML_FLAT_MAP_HPP_

#include <libmeme/Core/DS.hpp>

// Flat Map
namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Key, class Value, class Compare = std::less<Key>
	> struct flat_map
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type						= typename _ML_DS flat_map<Key, Value, Compare>;
		using key_type						= typename Key;
		using value_type					= typename Value;
		using compare_type					= typename Compare;

		using pair_type						= typename std::pair<key_type, value_type>;
		using initializer_type				= typename std::initializer_list<pair_type>;
		
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

		using iterator_pair					= typename std::pair<key_iterator, value_iterator>;
		using const_iterator_pair			= typename std::pair<const_key_iterator, const_value_iterator>;
		using reverse_iterator_pair			= typename std::pair<reverse_key_iterator, reverse_value_iterator>;
		using const_reverse_iterator_pair	= typename std::pair<const_reverse_key_iterator, const_reverse_value_iterator>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		flat_map() noexcept
			: m_keys{}, m_values{}
		{
		}

		flat_map(initializer_type init)
			: m_keys{}, m_values{}
		{
			for (auto it = init.begin(); it != init.end(); ++it)
			{
				emplace(it->first, it->second);
			}
		}

		flat_map(self_type const & other)
			: m_keys{ other.m_keys }, m_values{ other.m_values }
		{
		}

		flat_map(self_type && other) noexcept
			: m_keys{}, m_values{}
		{
			swap(std::move(other));
		}

		~flat_map() noexcept {}

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

		inline size_t capacity() const noexcept
		{
			return m_keys.capacity();
		}

		inline void clear() noexcept
		{
			m_keys.clear();
			m_values.clear();
		}

		inline bool empty() const noexcept
		{
			return m_keys.empty();
		}

		inline void reserve(size_t const count)
		{
			m_keys.reserve(count);
			m_values.resize(count);
		}

		inline void resize(size_t const count)
		{
			m_keys.resize(count);
			m_values.resize(count);
		}

		inline void resize(size_t const count, value_type const & value)
		{
			m_keys.resize(count);
			m_values.resize(count, value);
		}

		inline void shrink_to_fit()
		{
			m_keys.shrink_to_fit();
			m_values.shrink_to_fit();
		}

		inline size_t size() const noexcept
		{
			return m_keys.size();
		}

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_keys.swap(other.m_keys);
				m_values.swap(other.m_values);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto keys() const noexcept -> key_storage const & { return m_keys; }

		inline auto values() const noexcept -> value_storage const & { return m_values; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline bool contains(U && key) const
		{
			return m_keys.contains(std::forward<U>(key));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline value_iterator value_at(const_key_iterator key)
		{
			return (key != m_keys.end())
				? std::next(m_values.begin(), std::distance(m_keys.cbegin(), key))
				: m_values.end();
		}

		inline const_value_iterator value_at(const_key_iterator key) const
		{
			return (key != m_keys.end())
				? std::next(m_values.begin(), std::distance(m_keys.cbegin(), key))
				: m_values.end();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline value_iterator find(U && key)
		{
			return value_at(m_keys.find(std::forward<U>(key)));
		}

		template <class U> inline const_value_iterator find(U && key) const
		{
			return value_at(m_keys.find(std::forward<U>(key)));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator_pair erase(key_iterator location)
		{
			return { m_keys.erase(location), m_values.erase(value_at(location)) };
		}

		inline iterator_pair erase(key_iterator first, key_iterator last)
		{
			return { m_keys.erase(first, last), m_values.erase(value_at(first), value_at(last)) };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> inline std::pair<iterator_pair, bool> emplace(key_type const & key, Args && ... args)
		{
			if (auto const k{ m_keys.insert(key) }; !k.second)
			{
				return { { k.first, value_at(k.first) }, false };
			}
			else
			{
				return { { k.first, m_values.emplace(value_at(k.first), std::forward<Args>(args)...) }, true };
			}
		}

		template <class Key, class ... Args
		> inline std::pair<iterator_pair, bool> emplace(Key && key, Args && ... args)
		{
			if (auto const k{ m_keys.insert(std::move(key)) }; !k.second)
			{
				return { { k.first, value_at(k.first) }, false };
			}
			else
			{
				return { { k.first, m_values.emplace(value_at(k.first), std::forward<Args>(args)...) }, true };
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::pair<iterator_pair, bool> insert(pair_type const & value)
		{
			return emplace(value.first, value.second);
		}

		inline std::pair<iterator_pair, bool> insert(pair_type && value)
		{
			return emplace(std::move(value.first), std::move(value.second));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline value_type & at(key_type const & key)
		{
			if (auto const v{ find(key) }; v != m_values.end())
			{
				return (*v);
			}
			else
			{
				return (*emplace(key, value_type{}).first.second);
			}
		}

		inline value_type & operator[](key_type const & key)
		{
			return at(key);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> inline self_type & for_each(const_key_iterator first, const_key_iterator last, Fn fn)
		{
			if (!empty())
			{
				for (; first != last; ++first)
				{
					std::invoke(fn, *first, *value_at(first));
				}
			}
			return (*this);
		}

		template <class Fn
		> inline self_type & for_each(const_key_iterator first, Fn fn)
		{
			return for_each(first, m_keys.end(), fn);
		}

		template <class Fn
		> inline self_type & for_each(Fn fn)
		{
			return for_each(m_keys.begin(), fn);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> inline self_type const & for_each(const_key_iterator first, const_key_iterator last, Fn fn) const
		{
			if (!empty())
			{
				for (; first != last; ++first)
				{
					std::invoke(fn, *first, *value_at(first));
				}
			}
			return (*this);
		}

		template <class Fn
		> inline self_type const & for_each(const_key_iterator first, Fn fn) const
		{
			return for_each(first, m_keys.end(), fn);
		}

		template <class Fn
		> inline self_type const & for_each(Fn fn) const
		{
			return for_each(m_keys.begin(), fn);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> inline self_type & for_each_n(const_key_iterator first, ptrdiff_t count, Fn fn)
		{
			if (!empty() && (0 < count))
			{
				do {
					std::invoke(fn, *first, *value_at(first));
					--count;
					++first;
				} while (0 < count);
			}
			return (*this);
		}

		template <class Fn
		> inline self_type & for_each_n(ptrdiff_t count, Fn fn)
		{
			return for_each_n(m_keys.begin(), count, fn);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> inline self_type const & for_each_n(const_key_iterator first, ptrdiff_t count, Fn fn) const
		{
			if (!empty() && (0 < count))
			{
				do {
					std::invoke(fn, *first, *value_at(first));
					--count;
					++first;
				} while (0 < count);
			}
			return (*this);
		}

		template <class Fn
		> inline self_type const & for_each_n(ptrdiff_t count, Fn fn) const
		{
			return for_each_n(m_keys.begin(), count, fn);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		key_storage m_keys;

		value_storage m_values;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FLAT_MAP_HPP_