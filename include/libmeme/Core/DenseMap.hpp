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
		using value_type				= typename Pair;
		using compare_type				= typename Comp;
		using allocator_type			= typename Alloc;

		using self_type					= typename _ML dense_map<Key, Value, Comp, Pair, Alloc>;
		using storage_type				= typename std::vector<value_type, allocator_type>;
		using initializer_type			= typename std::initializer_list<value_type>;

		using pointer					= typename value_type *;
		using reference					= typename value_type &;
		using const_pointer				= typename value_type const *;
		using const_reference			= typename value_type const &;
		using rvalue_reference			= typename value_type &&;
		
		using difference_type			= typename storage_type::difference_type;
		using size_type					= typename storage_type::size_type;
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

		dense_map() noexcept
			: m_storage{}
		{
		}

		dense_map(initializer_type init)
			: m_storage{ init }
		{
			sort();
		}

		template <class It> dense_map(It first, It last)
			: m_storage{ first, last }
		{
			sort();
		}

		dense_map(storage_type const & value, allocator_type const & alloc = allocator_type{})
			: m_storage{ value, alloc }
		{
			sort();
		}

		dense_map(storage_type && value, allocator_type const & alloc = allocator_type{}) noexcept
			: m_storage{ std::move(value), alloc }
		{
			sort();
		}

		dense_map(self_type const & value)
			: m_storage{ value.m_storage }
		{
		}

		dense_map(self_type && value) noexcept
			: m_storage{}
		{
			swap(std::move(value));
		}

		~dense_map() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(self_type const & value)
		{
			self_type temp{ value };
			swap(temp);
			return (*this);
		}

		inline self_type & operator=(self_type && value) noexcept
		{
			swap(std::move(value));
			return (*this);
		}

		inline void swap(self_type & value) noexcept
		{
			if (this != std::addressof(value))
			{
				std::swap(m_storage, value.m_storage);
			}
		}

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

		inline iterator find(key_type const & key)
		{
			if (auto const it{ std::equal_range(begin(), end(), key, compare_impl{}) }
			; it.first != it.second)
			{
				return it.first;
			}
			return end();
		}

		inline const_iterator find(key_type const & key) const
		{
			if (auto const it{ std::equal_range(cbegin(), cend(), key, compare_impl{}) }
			; it.first != it.second)
			{
				return it.first;
			}
			return cend();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::pair<iterator, bool> insert(const_reference value)
		{
			if (auto const it{ std::equal_range(begin(), end(), value.first, compare_impl{}) }
			; it.first != it.second)
			{
				return std::make_pair(it.second, false);
			}
			else
			{
				return std::make_pair(m_storage.emplace(it.second, value), true);
			}
		}

		inline std::pair<iterator, bool> insert(rvalue_reference value)
		{
			if (auto const it{ std::equal_range(begin(), end(), value.first, compare_impl{}) }
			; it.first != it.second)
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
			if (auto const it{ std::equal_range(begin(), end(), key, compare_impl{}) }
			; it.first != it.second)
			{
				return it.first->second;
			}
			else
			{
				return m_storage.emplace(it.second, value_type{ key, mapped_type{} })->second;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void clear() noexcept { return m_storage.clear(); }

		inline void reserve(size_type const size) { return m_storage.reserve(size); }

		inline void sort() noexcept { return std::sort(begin(), end(), compare_impl{}); }

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
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_DENSE_MAP_HPP_