#ifndef _ML_DENSE_MAP_HPP_
#define _ML_DENSE_MAP_HPP_

#include <libmeme/Core/Core.hpp>

namespace ml
{
	// vector of pairs
	template <class Key, class Value,
		class Comp = std::less<Key>,
		class Pair = std::pair<Key, Value>,
		class Alloc = std::allocator<Pair>
	> struct dense_map
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using key_type					= typename Key;
		using mapped_type				= typename Value;
		using compare_type				= typename Comp;
		using value_type				= typename Pair;
		using allocator_type			= typename Alloc;

		using self_type					= typename _ML dense_map<Key, Value, Comp, Pair, Alloc>;
		using storage_type				= typename std::vector<value_type, allocator_type>;
		using initializer_type			= typename std::initializer_list<value_type>;

		using difference_type			= typename storage_type::difference_type;
		using size_type					= typename storage_type::size_type;
		using pointer					= typename storage_type::pointer;
		using reference					= typename storage_type::reference;
		using const_pointer				= typename storage_type::const_pointer;
		using const_reference			= typename storage_type::const_reference;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct compare_impl final
		{
			constexpr compare_impl() noexcept = default;

			constexpr bool operator()(const_reference lhs, key_type const & rhs) const
			{
				return compare_type{}(lhs.first, rhs);
			}

			constexpr bool operator()(key_type const & lhs, const_reference rhs) const
			{
				return compare_type{}(lhs, rhs.first);
			}

			constexpr bool operator()(const_reference lhs, const_reference rhs) const
			{
				return compare_type{}(lhs.first, rhs.first);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		dense_map() noexcept : m_storage{} {}

		~dense_map() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		dense_map(initializer_type init)
			: m_storage{ init }
		{
		}

		dense_map(initializer_type init, std::input_iterator_tag)
			: m_storage{ init, std::input_iterator_tag }
		{
		}

		dense_map(initializer_type init, std::forward_iterator_tag)
			: m_storage{ init, std::forward_iterator_tag }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class It> dense_map(It first, It last)
			: m_storage{ first, last }
		{
		}

		template <class It> dense_map(It first, It last, std::input_iterator_tag)
			: m_storage{ first, last, std::input_iterator_tag }
		{
		}

		template <class It> dense_map(It first, It last, std::forward_iterator_tag)
			: m_storage{ first, last, std::forward_iterator_tag }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		dense_map(storage_type const & value)
			: m_storage{ value }
		{
		}

		dense_map(storage_type const & value, allocator_type const & alloc)
			: m_storage{ value, alloc }
		{
		}

		dense_map(storage_type && value) noexcept
			: m_storage{ std::move(value) }
		{
		}

		dense_map(storage_type && value, allocator_type const & alloc) noexcept
			: m_storage{ std::move(value), alloc }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		dense_map(self_type const & value)
			: m_storage{ value.m_storage }
		{
		}

		dense_map(self_type && value) noexcept
			: m_storage{ std::move(value.m_storage) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(storage_type const & value)
		{
			assign(value);
			return (*this);
		}

		inline self_type & operator=(storage_type && value) noexcept
		{
			swap(std::move(value));
			return (*this);
		}

		inline self_type & operator=(self_type const & value)
		{
			assign(value);
			return (*this);
		}

		inline self_type & operator=(self_type && value) noexcept
		{
			swap(std::move(value));
			return (*this);
		}

		inline self_type & operator=(initializer_type value)
		{
			assign(value);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class It> inline void assign(It first, It last)
		{
			m_storage.assign(first, last);
		}

		inline void assign(initializer_type init)
		{
			m_storage.assign(init);
		}

		inline void assign(storage_type const & value)
		{
			if (std::addressof(m_storage) != std::addressof(value))
			{
				m_storage = value;
			}
		}

		inline void assign(self_type const & value)
		{
			if (this != std::addressof(value))
			{
				m_storage = value.m_storage;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void swap(storage_type & value) noexcept
		{
			if (std::addressof(m_storage) != std::addressof(value))
			{
				m_storage.swap(value);
			}
		}

		inline void swap(self_type & value) noexcept
		{
			if (this != std::addressof(value))
			{
				m_storage.swap(value.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void clear() noexcept { return m_storage.clear(); }

		inline void pop_back() noexcept { return m_storage.pop_back(); }

		inline void reserve(size_type const size) { return m_storage.reserve(size); }

		inline void resize(size_type const size) { return m_storage.resize(size); }

		inline void sort() noexcept { return std::sort(begin(), end(), compare_impl{}); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator erase(const_iterator it)
		{
			return m_storage.erase(it);
		}

		inline iterator erase(const_iterator first, const_iterator last)
		{
			return m_storage.erase(first, last);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline decltype(auto) find_range(key_type const & key)
		{
			return std::equal_range(begin(), end(), key, compare_impl{});
		}

		inline decltype(auto) find_range(key_type const & key) const
		{
			return std::equal_range(cbegin(), cend(), key, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator find(key_type const & key)
		{
			if (auto const it{ find_range(key) }; it.first != it.second)
			{
				return it.second;
			}
			return end();
		}

		inline const_iterator find(key_type const & key) const
		{
			if (auto const it{ find_range(key) }; it.first != it.second)
			{
				return it.second;
			}
			return cend();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::pair<iterator, bool> insert(const_reference value)
		{
			if (auto const it{ find_range(value.first) }; it.first != it.second)
			{
				return std::make_pair(it.second, false);
			}
			else
			{
				return std::make_pair(m_storage.insert(it.second, value), true);
			}
		}

		inline std::pair<iterator, bool> insert(value_type && value)
		{
			if (auto const it{ find_range(value.first) }; it.first != it.second)
			{
				return std::make_pair(it.second, false);
			}
			else
			{
				return std::make_pair(m_storage.emplace(it.second, std::move(value)), true);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline mapped_type & operator[](key_type const & key)
		{
			if (auto const it{ find_range(key) }; it.first != it.second)
			{
				return it.first->second;
			}
			else
			{
				return m_storage.emplace(it.second, std::make_pair(key, mapped_type{}))->second;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto back() noexcept -> reference { return m_storage.back(); }

		inline auto back() const noexcept -> const_reference { return m_storage.back(); }

		inline auto begin() noexcept -> iterator { return m_storage.begin(); }

		inline auto begin() const noexcept -> const_iterator { return m_storage.begin(); }

		inline auto capacity() const noexcept -> size_type { return m_storage.capacity(); }

		inline auto cbegin() const noexcept -> const_iterator { return m_storage.cbegin(); }

		inline auto cend() const noexcept -> const_iterator { return m_storage.cend(); }

		inline auto crbegin() const noexcept -> const_reverse_iterator { return m_storage.crbegin(); }

		inline auto crend() const noexcept -> const_reverse_iterator { return m_storage.crend(); }

		inline auto data() noexcept -> pointer { return m_storage.data(); }

		inline auto data() const noexcept -> const_pointer { return m_storage.data(); }

		inline bool empty() const noexcept { return m_storage.empty(); }

		inline auto end() noexcept -> iterator { return m_storage.end(); }

		inline auto end() const noexcept -> const_iterator { return m_storage.end(); }

		inline auto front() noexcept -> reference { return m_storage.front(); }

		inline auto front() const noexcept -> const_reference { return m_storage.front(); }

		inline auto get_allocator() const noexcept -> allocator_type { return m_storage.get_allocator(); }

		inline auto max_size() const noexcept -> size_type { return m_storage.max_size(); }

		inline auto rbegin() noexcept -> reverse_iterator { return m_storage.rbegin(); }

		inline auto rbegin() const noexcept -> const_reverse_iterator { return m_storage.rbegin(); }

		inline auto rend() noexcept -> reverse_iterator { return m_storage.rend(); }

		inline auto rend() const noexcept -> const_reverse_iterator { return m_storage.rend(); }

		inline auto size() const noexcept -> size_type { return m_storage.size(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		union { storage_type m_storage; };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_DENSE_MAP_HPP_