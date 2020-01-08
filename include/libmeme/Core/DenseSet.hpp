#ifndef _ML_DENSE_SET_HPP_
#define _ML_DENSE_SET_HPP_

#include <libmeme/Core/Dense.hpp>

namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Basic Dense Set Traits
	template <class Elem, class Comp, class Alloc, bool Multi
	> struct basic_set_traits : public basic_container_traits<
		Elem, Alloc, Comp
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename basic_container_traits<Elem, Alloc, Comp>;

		static constexpr bool multi{ Multi };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type				= typename base_type::value_type;
		using compare_type				= typename base_type::compare_type;
		using allocator_type			= typename base_type::allocator_type;
		using storage_type				= typename base_type::storage_type;
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
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// Basic Dense Ordered Set
	template <class Elem, class Comp, class Alloc
	> struct basic_ordered_set : public basic_container<
		basic_set_traits<Elem, Comp, Alloc, false>
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type					= typename basic_ordered_set<Elem, Comp, Alloc>;
		using traits_type				= typename basic_set_traits<Elem, Comp, Alloc, false>;
		using base_type					= typename basic_container<traits_type>;
		using value_type				= typename base_type::value_type;
		using compare_type				= typename base_type::compare_type;
		using allocator_type			= typename base_type::allocator_type;
		using storage_type				= typename base_type::storage_type;
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
			sort(); erase(std::unique(begin(), end(), compare_type{}), end());
		}

		template <class It> basic_ordered_set(It first, It last)
			: base_type{ first, last }
		{
			sort(); erase(std::unique(begin(), end(), compare_type{}), end());
		}

		explicit basic_ordered_set(allocator_type const & alloc)
			: base_type{ alloc }
		{
			sort(); erase(std::unique(begin(), end(), compare_type{}), end());
		}
	
		explicit basic_ordered_set(storage_type const & value, allocator_type const & alloc = {})
			: base_type{ value, alloc }
		{
			sort(); erase(std::unique(begin(), end(), compare_type{}), end());
		}
	
		explicit basic_ordered_set(storage_type && value, allocator_type const & alloc = {}) noexcept
			: base_type{ std::move(value), alloc }
		{
			sort(); erase(std::unique(begin(), end(), compare_type{}), end());
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

		inline self_type & operator=(initializer_type init)
		{
			self_type temp{ init };
			swap(temp);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U> inline iterator find(U && u)
		{
			if (auto const it{ equal_range(std::forward<U>(u)) }; it.first != it.second)
			{
				return it.first;
			}
			return end();
		}

		template <class U> inline const_iterator find(U && u) const
		{
			if (auto const it{ equal_range(std::forward<U>(u)) }; it.first != it.second)
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
				return std::make_pair(m_storage.emplace(it.second, value), true);
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
				return std::make_pair(m_storage.emplace(it.second, std::move(value)), true);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Basic Dense Ordered Multiset
	template <class Elem, class Comp, class Alloc
	> struct basic_ordered_multiset : public basic_container<
		basic_set_traits<Elem, Comp, Alloc, true>
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type					= typename basic_ordered_multiset<Elem, Comp, Alloc>;
		using traits_type				= typename basic_set_traits<Elem, Comp, Alloc, true>;
		using base_type					= typename basic_container<traits_type>;
		using value_type				= typename base_type::value_type;
		using compare_type				= typename base_type::compare_type;
		using allocator_type			= typename base_type::allocator_type;
		using storage_type				= typename base_type::storage_type;
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
			sort();
		}

		template <class It> basic_ordered_multiset(It first, It last)
			: base_type{ first, last }
		{
			sort();
		}

		explicit basic_ordered_multiset(allocator_type const & alloc)
			: base_type{ alloc }
		{
		}
	
		explicit basic_ordered_multiset(storage_type const & value, allocator_type const & alloc = {})
			: base_type{ value, alloc }
		{
			sort();
		}
	
		explicit basic_ordered_multiset(storage_type && value, allocator_type const & alloc = {}) noexcept
			: base_type{ std::move(value), alloc }
		{
			sort();
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

		inline iterator insert(const_reference value)
		{
			return m_storage.emplace(upper_bound(value), value);
		}

		inline iterator insert(value_type && value)
		{
			return m_storage.emplace(upper_bound(std::move(value)), std::move(value));
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
	template <class Elem
	> ML_USING ordered_set = _ML_DENSE basic_ordered_set<
		Elem,
		std::less<Elem>,
		std::allocator<Elem>
	>;

	// Ordered Multiset
	template <class Elem
	> ML_USING ordered_multiset = _ML_DENSE basic_ordered_multiset<
		Elem,
		std::less<Elem>,
		std::allocator<Elem>
	>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DENSE_SET_HPP_