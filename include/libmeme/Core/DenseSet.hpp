#ifndef _ML_DENSE_SET_HPP_
#define _ML_DENSE_SET_HPP_

#include <libmeme/Core/Dense.hpp>

// Basic Set Traits
namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class ...> class Derived,
		class Value, class Comp, class Alloc, bool Multi
	> struct basic_set_traits : public basic_storage_traits<Value, Alloc>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename basic_storage_traits<Value, Alloc>;

		using derived_type = typename Derived<Value, Comp, Alloc>;

		using compare_type = typename Comp;

		static constexpr bool multi{ Multi };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type			= typename base_type::allocator_type;
		using storage_type				= typename base_type::storage_type;
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
		using initializer_type			= typename base_type::initializer_type;
		using iterator_pair				= typename base_type::iterator_pair;
		using const_iterator_pair		= typename base_type::const_iterator_pair;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct compare_impl
		{
			constexpr compare_impl() noexcept = default;

			constexpr bool operator()(const_reference lhs, const_reference rhs) const noexcept
			{
				return compare_type{}(lhs, rhs);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct sort_impl
		{
			constexpr sort_impl() noexcept = default;

			inline void operator()(derived_type & value) const noexcept
			{
				std::sort(value.begin(), value.end(), compare_impl{});

				if constexpr (!multi)
				{
					value.erase(std::unique(value.begin(), value.end()), value.end(), compare_impl{});
				}
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Basic Ordered Set
namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <class Value,
		class Comp = std::less<Value>,
		class Alloc = std::allocator<Value>
	> struct basic_ordered_set : public basic_storage<basic_set_traits<
		basic_ordered_set, Value, Comp, Alloc, false
	>>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
		using traits_type = basic_set_traits<basic_ordered_set,
			Value, Comp, Alloc, false
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type					= typename basic_storage<traits_type>;
		using self_type					= typename traits_type::derived_type;
		using compare_impl				= typename traits_type::compare_impl;
		using allocator_type			= typename base_type::allocator_type;
		using storage_type				= typename base_type::storage_type;
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
		using initializer_type			= typename base_type::initializer_type;
		using iterator_pair				= typename base_type::iterator_pair;
		using const_iterator_pair		= typename base_type::const_iterator_pair;
	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
		basic_ordered_set() noexcept
			: base_type{}
		{
		}

		basic_ordered_set(initializer_type init)
			: base_type{ init }
		{
			traits_type::sort_impl{}(*this);
		}

		template <class It> basic_ordered_set(It first, It last)
			: base_type{ first, last }
		{
			traits_type::sort_impl{}(*this);
		}

		explicit basic_ordered_set(allocator_type const & alloc)
			: base_type{ alloc }
		{
			traits_type::sort_impl{}(*this);
		}
	
		explicit basic_ordered_set(storage_type const & value, allocator_type const & alloc = {})
			: base_type{ value, alloc }
		{
			traits_type::sort_impl{}(*this);
		}
	
		explicit basic_ordered_set(storage_type && value, allocator_type const & alloc = {}) noexcept
			: base_type{ std::move(value), alloc }
		{
			traits_type::sort_impl{}(*this);
		}
	
		basic_ordered_set(self_type const & other, allocator_type const & alloc = {})
			: base_type{ other, alloc }
		{
		}
	
		basic_ordered_set(self_type && other, allocator_type const & alloc = {}) noexcept
			: base_type{ std::move(other), alloc }
		{
		}

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Other> inline bool binary_search(Other && u) const
		{
			return std::binary_search(cbegin(), cend(), std::forward<Other>(u), compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Other> iterator_pair equal_range(Other && u)
		{
			return std::equal_range(begin(), end(), std::forward<Other>(u), compare_impl{});
		}

		template <class Other> const_iterator_pair equal_range(Other && u) const
		{
			return std::equal_range(cbegin(), cend(), std::forward<Other>(u), compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Other> iterator lower_bound(Other && u)
		{
			return std::lower_bound(begin(), end(), std::forward<Other>(u), compare_impl{});
		}

		template <class Other> const_iterator lower_bound(Other && u) const
		{
			return std::lower_bound(cbegin(), cend(), std::forward<Other>(u), compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Other> iterator upper_bound(Other && u)
		{
			return std::upper_bound(begin(), end(), std::forward<Other>(u), compare_impl{});
		}

		template <class Other> const_iterator upper_bound(Other && u) const
		{
			return std::upper_bound(cbegin(), cend(), std::forward<Other>(u), compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Other> inline iterator find(Other && u)
		{
			if (auto const it{ equal_range(std::forward<Other>(u)) }; it.first != it.second)
			{
				return it.first;
			}
			return end();
		}

		template <class Other> inline const_iterator find(Other && u) const
		{
			if (auto const it{ equal_range(std::forward<Other>(u)) }; it.first != it.second)
			{
				return it.first;
			}
			return cend();
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
				return std::make_pair(emplace(it.second, value), true);
			}
		}

		inline std::pair<iterator, bool> insert(value_type && value)
		{
			if (auto const it{ equal_range(std::move(value)) }; it.first != it.second)
			{
				return std::make_pair(it.second, false);
			}
			else
			{
				return std::make_pair(emplace(it.second, std::move(value)), true);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Basic Ordered Multiset
namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Value,
		class Comp = std::less<Value>,
		class Alloc = std::allocator<Value>
	> struct basic_ordered_multiset : public basic_storage<basic_set_traits<
		basic_ordered_multiset, Value, Comp, Alloc, true
	>>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
		using traits_type = basic_set_traits<basic_ordered_multiset,
			Value, Comp, Alloc, true
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type					= typename basic_storage<traits_type>;
		using self_type					= typename traits_type::derived_type;
		using compare_impl				= typename traits_type::compare_impl;
		using allocator_type			= typename base_type::allocator_type;
		using storage_type				= typename base_type::storage_type;
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
		using initializer_type			= typename base_type::initializer_type;
		using iterator_pair				= typename base_type::iterator_pair;
		using const_iterator_pair		= typename base_type::const_iterator_pair;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
		basic_ordered_multiset() noexcept
			: base_type{}
		{
		}

		basic_ordered_multiset(initializer_type init)
			: base_type{ init }
		{
			traits_type::sort_impl{}(*this);
		}

		template <class It> basic_ordered_multiset(It first, It last)
			: base_type{ first, last }
		{
			traits_type::sort_impl{}(*this);
		}

		explicit basic_ordered_multiset(allocator_type const & alloc)
			: base_type{ alloc }
		{
		}
	
		explicit basic_ordered_multiset(storage_type const & value, allocator_type const & alloc = {})
			: base_type{ value, alloc }
		{
			traits_type::sort_impl{}(*this);
		}
	
		explicit basic_ordered_multiset(storage_type && value, allocator_type const & alloc = {}) noexcept
			: base_type{ std::move(value), alloc }
		{
			traits_type::sort_impl{}(*this);
		}
	
		basic_ordered_multiset(self_type const & other, allocator_type const & alloc = {})
			: base_type{ other, alloc }
		{
		}
	
		basic_ordered_multiset(self_type && other, allocator_type const & alloc = {}) noexcept
			: base_type{ std::move(other), alloc }
		{
		}

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

		template <class Other> inline bool binary_search(Other && u) const
		{
			return std::binary_search(cbegin(), cend(), std::forward<Other>(u), compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Other> iterator_pair equal_range(Other && u)
		{
			return std::equal_range(begin(), end(), std::forward<Other>(u), compare_impl{});
		}

		template <class Other> const_iterator_pair equal_range(Other && u) const
		{
			return std::equal_range(cbegin(), cend(), std::forward<Other>(u), compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Other> iterator lower_bound(Other && u)
		{
			return std::lower_bound(begin(), end(), std::forward<Other>(u), compare_impl{});
		}

		template <class Other> const_iterator lower_bound(Other && u) const
		{
			return std::lower_bound(cbegin(), cend(), std::forward<Other>(u), compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Other> iterator upper_bound(Other && u)
		{
			return std::upper_bound(begin(), end(), std::forward<Other>(u), compare_impl{});
		}

		template <class Other> const_iterator upper_bound(Other && u) const
		{
			return std::upper_bound(cbegin(), cend(), std::forward<Other>(u), compare_impl{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Other> inline iterator find(Other && u)
		{
			if (auto const it{ equal_range(std::forward<Other>(u)) }; it.first != it.second)
			{
				return it.first;
			}
			return end();
		}

		template <class Other> inline const_iterator find(Other && u) const
		{
			if (auto const it{ equal_range(std::forward<Other>(u)) }; it.first != it.second)
			{
				return it.first;
			}
			return cend();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator insert(const_reference value)
		{
			return emplace(upper_bound(value), value);
		}

		inline iterator insert(value_type && value)
		{
			return emplace(upper_bound(std::move(value)), std::move(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Usings
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Ordered Set
	template <class Value
	> ML_USING ordered_set = _ML_DENSE basic_ordered_set<
		Value,
		std::less<Value>,
		std::allocator<Value>
	>;

	// Ordered Multiset
	template <class Value
	> ML_USING ordered_multiset = _ML_DENSE basic_ordered_multiset<
		Value,
		std::less<Value>,
		std::allocator<Value>
	>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DENSE_SET_HPP_