#ifndef _ML_DENSE_SET_HPP_
#define _ML_DENSE_SET_HPP_

#include <libmeme/Core/Dense.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Basic Set Traits
	template <template <class ...> class Derived,
		class Value, class Comp, class Alloc, bool Multi
	> struct basic_set_traits : public _ML_DENSE basic_storage_traits<
		Value, Alloc
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using derived_type = typename Derived<Value, Comp, Alloc>;

		friend derived_type;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename _ML_DENSE basic_storage_traits<Value, Alloc>;

		using compare_type = typename Comp;

		using search_type = typename Value const &;

		static constexpr bool multi{ Multi };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using allocator_type			= typename base_type::allocator_type;
		using storage_type				= typename base_type::storage_type;
		using initializer_type			= typename base_type::initializer_type;
		using value_type				= typename base_type::value_type;
		using pointer					= typename base_type::pointer;
		using reference					= typename base_type::reference;
		using const_pointer				= typename base_type::const_pointer;
		using const_reference			= typename base_type::const_reference;
		using difference_type			= typename base_type::difference_type;
		using size_type					= typename base_type::size_type;
		using iterator					= typename base_type::iterator;
		using const_iterator			= typename base_type::const_iterator;
		using reverse_iterator			= typename base_type::reverse_iterator;
		using const_reverse_iterator	= typename base_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		struct compare_impl final
		{
			constexpr compare_impl() noexcept = default;

			constexpr bool operator()(search_type lhs, const_reference rhs) const noexcept
			{
				return compare_type{}(lhs, rhs);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct sort_impl final
		{
			explicit sort_impl(derived_type & value) noexcept { (*this)(value); }

			constexpr sort_impl() noexcept = default;

			inline void operator()(derived_type & value) const noexcept
			{
				std::sort(value.begin(), value.end(), compare_impl{});

				if constexpr (!multi)
				{
					value.erase(
						std::unique(value.begin(), value.end()), value.end(), compare_impl{}
					);
				}
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Basic Ordered Set
	template <
		class Value,
		class Comp = std::less<Value>,
		class Alloc = std::allocator<Value>
	> struct basic_ordered_set : public _ML_DENSE basic_storage<
		basic_set_traits<basic_ordered_set, Value, Comp, Alloc, false>
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
		using traits_type = typename basic_set_traits<
			basic_ordered_set, Value, Comp, Alloc, false
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type					= typename traits_type::derived_type;
		using compare_impl				= typename traits_type::compare_impl;
		using search_type				= typename traits_type::search_type;
		using base_type					= typename _ML_DENSE basic_storage<traits_type>;
		using allocator_type			= typename base_type::allocator_type;
		using storage_type				= typename base_type::storage_type;
		using initializer_type			= typename base_type::initializer_type;
		using value_type				= typename base_type::value_type;
		using pointer					= typename base_type::pointer;
		using reference					= typename base_type::reference;
		using const_pointer				= typename base_type::const_pointer;
		using const_reference			= typename base_type::const_reference;
		using difference_type			= typename base_type::difference_type;
		using size_type					= typename base_type::size_type;
		using iterator					= typename base_type::iterator;
		using const_iterator			= typename base_type::const_iterator;
		using reverse_iterator			= typename base_type::reverse_iterator;
		using const_reverse_iterator	= typename base_type::const_reverse_iterator;
	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
		basic_ordered_set() noexcept
			: base_type{ m_storage }, m_storage{}
		{
		}

		basic_ordered_set(initializer_type init)
			: base_type{ m_storage }, m_storage{ init }
		{
			traits_type::sort_impl{ *this };
		}

		template <class It> basic_ordered_set(It first, It last)
			: base_type{ m_storage }, m_storage{ first, last }
		{
			traits_type::sort_impl{ *this };
		}

		explicit basic_ordered_set(allocator_type const & alloc)
			: base_type{ m_storage }, m_storage{ alloc }
		{
			traits_type::sort_impl{ *this };
		}
	
		explicit basic_ordered_set(storage_type const & value, allocator_type const & alloc)
			: base_type{ m_storage }, m_storage{ value, alloc }
		{
			traits_type::sort_impl{ *this };
		}
	
		explicit basic_ordered_set(storage_type && value, allocator_type const & alloc) noexcept
			: base_type{ m_storage }, m_storage{ std::move(value), alloc }
		{
			traits_type::sort_impl{ *this };
		}
	
		basic_ordered_set(self_type const & other, allocator_type const & alloc = {})
			: base_type{ m_storage }, m_storage{ other.m_storage, alloc }
		{
		}
	
		basic_ordered_set(self_type && other) noexcept
			: base_type{ m_storage }, m_storage{}
		{
			swap(std::move(other));
		}

		~basic_ordered_set() noexcept {}

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

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator lower_bound(search_type value)
		{
			return std::lower_bound(begin(), end(), value, compare_impl{});
		}

		inline const_iterator lower_bound(search_type value) const
		{
			return std::lower_bound(cbegin(), cend(), value, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator upper_bound(search_type value)
		{
			return std::upper_bound(begin(), end(), value, compare_impl{});
		}

		inline const_iterator upper_bound(search_type value) const
		{
			return std::upper_bound(cbegin(), cend(), value, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::pair<iterator, iterator> equal_range(search_type value)
		{
			return std::equal_range(begin(), end(), value, compare_impl{});
		}

		inline std::pair<const_iterator, const_iterator> equal_range(search_type value) const
		{
			return std::equal_range(cbegin(), cend(), value, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator find(search_type value)
		{
			if (auto const it{ equal_range(value) }; it.first != it.second)
			{
				return it.first;
			}
			else
			{
				return end();
			}
		}

		inline const_iterator find(search_type value) const
		{
			if (auto const it{ equal_range(value) }; it.first != it.second)
			{
				return it.first;
			}
			else
			{
				return cend();
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::pair<iterator, bool> insert(const_reference value)
		{
			if (auto const it{ equal_range(value) }; it.first != it.second)
			{
				return std::make_pair(it.second, false);
			}
			else
			{
				return std::make_pair(m_storage.emplace(it.second, value), true);
			}
		}

		inline std::pair<iterator, bool> insert(value_type && value)
		{
			if (auto const it{ equal_range(value) }; it.first != it.second)
			{
				return std::make_pair(it.second, false);
			}
			else
			{
				return std::make_pair(m_storage.emplace(it.second, std::move(value)), true);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Basic Ordered Multiset
	template <
		class Value,
		class Comp = std::less<Value>,
		class Alloc = std::allocator<Value>
	> struct basic_ordered_multiset : public _ML_DENSE basic_storage<
		basic_set_traits<basic_ordered_multiset, Value, Comp, Alloc, true>
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
		using traits_type = typename basic_set_traits<
			basic_ordered_multiset, Value, Comp, Alloc, true
		>;
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using self_type					= typename traits_type::derived_type;
		using compare_impl				= typename traits_type::compare_impl;
		using search_type				= typename traits_type::search_type;
		using base_type					= typename _ML_DENSE basic_storage<traits_type>;
		using allocator_type			= typename base_type::allocator_type;
		using storage_type				= typename base_type::storage_type;
		using initializer_type			= typename base_type::initializer_type;
		using value_type				= typename base_type::value_type;
		using pointer					= typename base_type::pointer;
		using reference					= typename base_type::reference;
		using const_pointer				= typename base_type::const_pointer;
		using const_reference			= typename base_type::const_reference;
		using difference_type			= typename base_type::difference_type;
		using size_type					= typename base_type::size_type;
		using iterator					= typename base_type::iterator;
		using const_iterator			= typename base_type::const_iterator;
		using reverse_iterator			= typename base_type::reverse_iterator;
		using const_reverse_iterator	= typename base_type::const_reverse_iterator;
	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
		basic_ordered_multiset() noexcept
			: base_type{ m_storage }, m_storage{}
		{
		}

		basic_ordered_multiset(initializer_type init)
			: base_type{ m_storage }, m_storage{ init }
		{
			traits_type::sort_impl{ *this };
		}

		template <class It> basic_ordered_multiset(It first, It last)
			: base_type{ m_storage }, m_storage{ first, last }
		{
			traits_type::sort_impl{ *this };
		}

		explicit basic_ordered_multiset(allocator_type const & alloc)
			: base_type{ m_storage }, m_storage{ alloc }
		{
		}
	
		explicit basic_ordered_multiset(storage_type const & value, allocator_type const & alloc)
			: base_type{ m_storage }, m_storage{ value, alloc }
		{
			traits_type::sort_impl{ *this };
		}
	
		explicit basic_ordered_multiset(storage_type && value, allocator_type const & alloc) noexcept
			: base_type{ m_storage }, m_storage{ std::move(value), alloc }
		{
			traits_type::sort_impl{ *this };
		}
	
		basic_ordered_multiset(self_type const & other, allocator_type const & alloc = {})
			: base_type{ m_storage }, m_storage{ other.m_storage, alloc }
		{
		}
	
		basic_ordered_multiset(self_type && other) noexcept
			: base_type{ m_storage }, m_storage{}
		{
			swap(std::move(other));
		}

		~basic_ordered_multiset() noexcept {}

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

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator lower_bound(search_type value)
		{
			return std::lower_bound(begin(), end(), value, compare_impl{});
		}

		inline const_iterator lower_bound(search_type value) const
		{
			return std::lower_bound(cbegin(), cend(), value, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator upper_bound(search_type value)
		{
			return std::upper_bound(begin(), end(), value, compare_impl{});
		}

		inline const_iterator upper_bound(search_type value) const
		{
			return std::upper_bound(cbegin(), cend(), value, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::pair<iterator, iterator> equal_range(search_type value)
		{
			return std::equal_range(begin(), end(), value, compare_impl{});
		}

		inline std::pair<const_iterator, const_iterator> equal_range(search_type value) const
		{
			return std::equal_range(cbegin(), cend(), value, compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator find(search_type value)
		{
			if (auto const it{ equal_range(value) }; it.first != it.second)
			{
				return it.first;
			}
			else
			{
				return end();
			}
		}

		inline const_iterator find(search_type value) const
		{
			if (auto const it{ equal_range(value) }; it.first != it.second)
			{
				return it.first;
			}
			else
			{
				return cend();
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator insert(const_reference value)
		{
			return m_storage.emplace(equal_range(value).second, value);
		}

		inline iterator insert(value_type && value)
		{
			return m_storage.emplace(equal_range(value).second, std::move(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Ordered Set
	template <class Value
	> ML_USING ordered_set = basic_ordered_set<
		Value,
		std::less<Value>,
		std::allocator<Value>
	>;

	// Ordered Multiset
	template <class Value
	> ML_USING ordered_multiset = basic_ordered_multiset<
		Value,
		std::less<Value>,
		std::allocator<Value>
	>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DENSE_SET_HPP_