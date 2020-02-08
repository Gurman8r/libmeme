#ifndef _ML_FLAT_SET_HPP_
#define _ML_FLAT_SET_HPP_

#include <libmeme/Common.hpp>

namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// FLAT SET TRAITS
	template <
		class	_Ty,	// value type
		class	_Pr,	// comparator predicate type
		bool	_Mt,	// true if multiple equivalent values are permitted
		size_t	_Th		// threshold for selecting search algorithm
	> struct flat_set_traits final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename _Ty;
		using compare_type		= typename _Pr;
		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using difference_type	= typename ptrdiff_t;
		using size_type			= typename size_t;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr bool multi{ _Mt };

		static constexpr size_type thresh{ 42 };

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
			// empty
			if (empty()) { return false; }

			// linear
			if (size() < traits_type::thresh)
			{
				return cend() != std::find(cbegin(), cend(), other);
			}

			// binary
			return std::binary_search(cbegin(), cend(), other, compare_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Other = value_type
		> ML_NODISCARD inline iterator find(Other const & other)
		{
			// empty
			if (empty()) { return end(); }

			// linear
			if (size() < traits_type::thresh)
			{
				return std::find(begin(), end(), other);
			}

			// binary
			if (auto const it{ std::equal_range(begin(), end(), other, compare_type{}) }
			; it.first != it.second)
			{
				return it.first;
			}
			return end();
		}

		template <class Other = value_type
		> ML_NODISCARD inline const_iterator find(Other const & other) const
		{
			// empty
			if (empty()) { return cend(); }

			// linear
			if (size() < traits_type::thresh)
			{
				return std::find(cbegin(), cend(), other);
			}

			// binary
			if (auto const it{ std::equal_range(cbegin(), cend(), other, compare_type{}) }
			; it.first != it.second)
			{
				return it.first;
			}
			return cend();
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
			return this->impl_insert(ML_FWD(other));
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
		inline void impl_sort() noexcept
		{
			// empty
			if (empty()) return;

			// sort
			std::sort(begin(), end(), compare_type{});

			// remove duplicates
			if constexpr (!traits_type::multi)
			{
				erase(std::unique(begin(), end()), end());
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		template <class T> inline auto impl_insert(T && other)
		{
			if constexpr (traits_type::multi)
			{
				// insert multi
				return m_storage.emplace(
					std::upper_bound(begin(), end(), other, compare_type{}),
					ML_FWD(other));
			}
			else
			{
				// insert unique
				if (auto const it{ std::equal_range(begin(), end(), other, compare_type{}) }
				; it.first == it.second)
				{
					return std::make_pair(m_storage.emplace(it.second, ML_FWD(other)), true);
				}
				else
				{
					return std::make_pair(it.second, false);
				}
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	/* FLAT SET
	sorted vector of unique elements */
	template <
		class	_Ty,
		class	_Pr = std::less<_Ty>,
		size_t	_Th = 42
	> ML_USING flat_set = typename basic_flat_set<
		flat_set_traits<_Ty, _Pr, false, _Th>
	>;

	/* FLAT MULTISET
	sorted vector of elements which allows duplicates */
	template <
		class	_Ty,
		class	_Pr = std::less<_Ty>,
		size_t	_Th = 42
	> ML_USING flat_multiset = typename basic_flat_set<
		flat_set_traits<_Ty, _Pr, true, _Th>
	>;
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FLAT_SET_HPP_