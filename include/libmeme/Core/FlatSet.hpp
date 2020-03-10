#ifndef _ML_FLAT_SET_HPP_
#define _ML_FLAT_SET_HPP_

#include <libmeme/Core/Utility.hpp>

namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// FLAT SET TRAITS
	template <
		class	_Vt,	// value type
		class	_Pr,	// comparator predicate type
		bool	_Mt,	// true if multiple equivalent values are permitted
		size_t	_Th		// algorithm selector threshold
	> struct flat_set_traits final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename _Vt;
		using compare_type		= typename _Pr;
		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using difference_type	= typename ptrdiff_t;
		using size_type			= typename size_t;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr bool multi{ _Mt };

		static constexpr size_type thresh{ _Th };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using storage_type				= typename pmr::vector<value_type>;
		using pointer					= typename storage_type::pointer;
		using reference					= typename storage_type::reference;
		using const_pointer				= typename storage_type::const_pointer;
		using const_reference			= typename storage_type::const_reference;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;
		using iterator_pair				= typename std::pair<iterator, iterator>;
		using const_iterator_pair		= typename std::pair<const_iterator, const_iterator>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// BASIC FLAT SET
	template <class _Traits
	> struct basic_flat_set final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using traits_type				= typename _Traits;
		using self_type					= typename basic_flat_set<traits_type>;
		using value_type				= typename traits_type::value_type;
		using compare_type				= typename traits_type::compare_type;
		using allocator_type			= typename traits_type::allocator_type;
		using difference_type			= typename traits_type::difference_type;
		using size_type					= typename traits_type::size_type;

		using storage_type				= typename traits_type::storage_type;
		using pointer					= typename traits_type::pointer;
		using reference					= typename traits_type::reference;
		using const_pointer				= typename traits_type::const_pointer;
		using const_reference			= typename traits_type::const_reference;
		using iterator					= typename traits_type::iterator;
		using const_iterator			= typename traits_type::const_iterator;
		using reverse_iterator			= typename traits_type::reverse_iterator;
		using const_reverse_iterator	= typename traits_type::const_reverse_iterator;
		using iterator_pair				= typename traits_type::iterator_pair;
		using const_iterator_pair		= typename traits_type::const_iterator_pair	;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_flat_set() noexcept
			: m_storage{}
		{
		}

		explicit basic_flat_set(allocator_type const & alloc) noexcept
			: m_storage{ alloc }
		{
		}

		basic_flat_set(std::initializer_list<value_type> init, allocator_type const & alloc = {})
			: m_storage{ init, alloc }
		{
			impl_sort();
		}

		template <class It
		> basic_flat_set(It first, It last, allocator_type const & alloc = {})
			: m_storage{ first, last, alloc }
		{
			impl_sort();
		}

		explicit basic_flat_set(storage_type const & value, allocator_type const & alloc = {})
			: m_storage{ value, alloc }
		{
			impl_sort();
		}

		explicit basic_flat_set(storage_type && value, allocator_type const & alloc = {}) noexcept
			: m_storage{ std::move(value), alloc }
		{
			impl_sort();
		}

		basic_flat_set(self_type const & other, allocator_type const & alloc = {})
			: m_storage{ other.m_storage, alloc }
		{
		}

		basic_flat_set(self_type && other, allocator_type const & alloc = {}) noexcept
			: m_storage{ std::move(other.m_storage), alloc }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(std::initializer_list<value_type> init)
		{
			self_type temp{ init };
			swap(temp);
			return (*this);
		}

		inline storage_type & operator=(storage_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		inline storage_type & operator=(storage_type && other) noexcept
		{
			self_type temp{ std::move(other) };
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

		inline void clear() noexcept
		{
			m_storage.clear();
		}

		inline void reserve(size_type const cap)
		{
			m_storage.reserve(cap);
		}

		inline void resize(size_type const cap)
		{
			m_storage.resize(cap);
		}

		inline void shrink_to_fit()
		{
			m_storage.shrink_to_fit();
		}

		inline void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				m_storage.swap(other.m_storage);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator erase(iterator it)
		{
			return m_storage.erase(it);
		}

		inline iterator erase(iterator first, iterator last)
		{
			return m_storage.erase(first, last);
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

		template <class Other = value_type
		> ML_NODISCARD inline bool contains(Other const & other) const
		{
			return this->impl_contains(begin(), end(), other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Other = value_type
		> ML_NODISCARD inline iterator find(Other const & other)
		{
			return this->impl_find(begin(), end(), other);
		}

		template <class Other = value_type
		> ML_NODISCARD inline const_iterator find(Other const & other) const
		{
			return this->impl_find(cbegin(), cend(), other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Other = value_type
		> ML_NODISCARD inline auto insert(Other const & other)
		{
			return this->impl_insert(other);
		}

		template <class Other = value_type
		> ML_NODISCARD inline auto insert(Other && other)
		{
			return this->impl_insert(std::move(other));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto operator[](size_type const i) noexcept -> reference { return m_storage[i]; }

		ML_NODISCARD inline auto operator[](size_type const i) const noexcept -> const_reference { return m_storage[i]; }

		ML_NODISCARD inline auto at(size_type const i) -> reference { return m_storage.at(i); }

		ML_NODISCARD inline auto at(size_type const i) const -> const_reference { return m_storage.at(i); }

		ML_NODISCARD inline auto back() noexcept -> reference { return m_storage.back(); }

		ML_NODISCARD inline auto back() const noexcept -> const_reference { return m_storage.back(); }

		ML_NODISCARD inline auto begin() noexcept -> iterator { return m_storage.begin(); }

		ML_NODISCARD inline auto begin() const noexcept -> const_iterator { return m_storage.begin(); }

		ML_NODISCARD inline auto capacity() const noexcept -> size_type { return m_storage.capacity(); }

		ML_NODISCARD inline auto cbegin() const noexcept -> const_iterator { return m_storage.cbegin(); }

		ML_NODISCARD inline auto cend() const noexcept -> const_iterator { return m_storage.cend(); }

		ML_NODISCARD inline auto crbegin() const noexcept -> const_reverse_iterator { return m_storage.crbegin(); }

		ML_NODISCARD inline auto crend() const noexcept -> const_reverse_iterator { return m_storage.crend(); }

		ML_NODISCARD inline auto data() noexcept -> pointer { return m_storage.data(); }

		ML_NODISCARD inline auto data() const noexcept -> const_pointer { return m_storage.data(); }

		ML_NODISCARD inline bool empty() const noexcept { return m_storage.empty(); }

		ML_NODISCARD inline auto end() noexcept -> iterator { return m_storage.end(); }

		ML_NODISCARD inline auto end() const noexcept -> const_iterator { return m_storage.end(); }

		ML_NODISCARD inline auto front() noexcept -> reference { return m_storage.front(); }

		ML_NODISCARD inline auto front() const noexcept -> const_reference { return m_storage.front(); }

		ML_NODISCARD inline auto get_allocator() const noexcept -> allocator_type { return m_storage.get_allocator(); }

		ML_NODISCARD inline auto max_size() const noexcept -> size_type { return m_storage.max_size(); }

		ML_NODISCARD inline auto rbegin() noexcept -> reverse_iterator { return m_storage.rbegin(); }

		ML_NODISCARD inline auto rbegin() const noexcept -> const_reverse_iterator { return m_storage.rbegin(); }

		ML_NODISCARD inline auto rend() noexcept -> reverse_iterator { return m_storage.rend(); }

		ML_NODISCARD inline auto rend() const noexcept -> const_reverse_iterator { return m_storage.rend(); }

		ML_NODISCARD inline auto size() const noexcept -> size_type { return m_storage.size(); }

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

		template <class It, class Other
		> static inline bool impl_contains(It first, It last, Other const & other)
		{
			// linear
			auto impl_contains_linear = [&]()
			{
				return std::find(first, last, other) != last;
			};

			// binary
			auto impl_contains_binary = [&]()
			{
				return std::binary_search(first, last, other, compare_type{});
			};

			// impl
			if constexpr (traits_type::thresh == 0)
			{
				return impl_contains_linear();
			}
			else if (auto const size{ std::distance(first, last) })
			{
				if (size < traits_type::thresh)
				{
					return impl_contains_linear();
				}
				else
				{
					return impl_contains_binary();
				}
			}
			else
			{
				return false;
			}
		}

		// find implementation
		template <class It, class Other
		> static inline auto impl_find(It first, It last, Other const & other)
		{
			// linear
			auto impl_find_linear = [&]()
			{
				return std::find(first, last, other);
			};

			// binary
			auto impl_find_binary = [&]()
			{
				if (auto const it{ std::equal_range(first, last, other, compare_type{}) }
				; it.first != it.second)
				{
					return it.first;
				}
				else
				{
					return last;
				}
			};

			// impl
			if constexpr (traits_type::thresh == 0)
			{
				return impl_find_linear();
			}
			else if (auto const size{ std::distance(first, last) })
			{
				if (size < traits_type::thresh)
				{
					return impl_find_linear();
				}
				else
				{
					return impl_find_binary();
				}
			}
			else
			{
				return last;
			}
		}
		
		// sort implementation
		inline void impl_sort() noexcept
		{
			// empty
			if (empty()) return;

			// sort
			if constexpr (0 < traits_type::thresh)
			{
				std::sort(begin(), end(), compare_type{});
			}

			// remove duplicates
			if constexpr (!traits_type::multi)
			{
				this->erase(std::unique(begin(), end()), end());
			}
		}

		// insert implementation
		template <class Other
		> inline auto impl_insert(Other && other) -> std::conditional_t<
			traits_type::multi, iterator, std::pair<iterator, bool>
		>
		{
			if constexpr (traits_type::multi)
			{
				// insert multi
				return m_storage.emplace(
					std::upper_bound(begin(), end(), other, compare_type{}),
					ML_forward(other)
				);
			}
			else
			{
				// insert unique
				if (auto const it{ std::equal_range(begin(), end(), other, compare_type{}) }
				; it.first == it.second)
				{
					return { m_storage.emplace(it.second, ML_forward(other)), true };
				}
				else
				{
					return { it.second, false };
				}
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// FLAT SET | sorted vector of unique elements
	template <
		class	_Vt,					// value type
		class	_Pr = std::less<_Vt>,	// comparator predicate type
		size_t	_Th = 42				// algorithm selector threshold
	> ML_ALIAS flat_set = typename basic_flat_set
	<
		flat_set_traits<_Vt, _Pr, false, _Th>
	>;

	// FLAT MULTISET | sorted vector of elements
	template <
		class	_Vt,					// value type
		class	_Pr = std::less<_Vt>,	// comparator predicate type
		size_t	_Th = 42				// algorithm selector threshold
	> ML_ALIAS flat_multiset = typename basic_flat_set
	<
		flat_set_traits<_Vt, _Pr, true, _Th>
	>;
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FLAT_SET_HPP_