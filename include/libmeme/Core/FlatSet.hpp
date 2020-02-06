#ifndef _ML_FLAT_SET_HPP_
#define _ML_FLAT_SET_HPP_

#include <libmeme/Common.hpp>

namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// BASIC FLAT SET TRAITS
	template <
		class	_Ty,	// value type
		class	_Pr,	// comparator predicate type
		bool	_Multi,	// true if multiple equivalent keys are permitted
		size_t	_Thresh	// size threshold used to determine search algorithm
	> struct basic_flat_set_traits
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type = typename _Ty;
		
		using compare_type = typename _Pr;
		
		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		static constexpr bool multi{ _Multi };

		static constexpr size_t thresh{ _Thresh };

		using difference_type = typename ptrdiff_t;

		using size_type = typename size_t;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// BASIC FLAT SET
	template <class _Traits
	> struct basic_flat_set
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using traits_type				= typename _Traits;
		using self_type					= typename basic_flat_set<traits_type>;
		using value_type				= typename traits_type::value_type;
		using compare_type				= typename traits_type::compare_type;
		using allocator_type			= typename traits_type::allocator_type;
		using difference_type			= typename traits_type::difference_type;
		using size_type					= typename traits_type::size_type;

		using storage_type				= typename pmr::vector<value_type>;
		using pointer					= typename storage_type::pointer;
		using reference					= typename storage_type::reference;
		using const_pointer				= typename storage_type::const_pointer;
		using const_reference			= typename storage_type::const_reference;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;
		using initializer_type			= typename std::initializer_list<value_type>;
		using iterator_pair				= typename std::pair<iterator, iterator>;
		using const_iterator_pair		= typename std::pair<const_iterator, const_iterator>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		basic_flat_set() noexcept
			: m_storage{}
		{
		}

		explicit basic_flat_set(allocator_type const & alloc) noexcept
			: m_storage{ alloc }
		{
		}

		basic_flat_set(initializer_type init, allocator_type const & alloc = {})
			: m_storage{ init, alloc }
		{
		}

		template <class It
		> basic_flat_set(It first, It last, allocator_type const & alloc = {})
			: m_storage{ first, last, alloc }
		{
		}

		explicit basic_flat_set(storage_type const & value, allocator_type const & alloc = {})
			: m_storage{ value, alloc }
		{
		}

		explicit basic_flat_set(storage_type && value, allocator_type const & alloc = {}) noexcept
			: m_storage{ std::move(value), alloc }
		{
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

		inline void clear() noexcept
		{
			m_storage.clear();
		}

		inline void reserve(size_type const count)
		{
			m_storage.reserve(count);
		}

		inline void resize(size_type const count)
		{
			m_storage.resize(count);
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

		inline iterator erase(iterator location)
		{
			return m_storage.erase(location);
		}

		inline iterator erase(iterator first, iterator last)
		{
			return m_storage.erase(first, last);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline bool contains(const_reference value) const
		{
			if (size() < traits_type::thresh)
			{
				// linear
				return std::find(cbegin(), cend(), value) != cend();
			}
			else
			{
				// binary
				return std::binary_search(cbegin(), cend(), value, compare_type{});
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline iterator_pair equal_range(const_reference value)
		{
			return std::equal_range(begin(), end(), value, compare_type{});
		}

		ML_NODISCARD inline const_iterator_pair equal_range(const_reference value) const
		{
			return std::equal_range(cbegin(), cend(), value, compare_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline iterator lower_bound(const_reference value)
		{
			return std::lower_bound(begin(), end(), value, compare_type{});
		}

		ML_NODISCARD inline const_iterator lower_bound(const_reference value) const
		{
			return std::lower_bound(cbegin(), cend(), value, compare_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline iterator upper_bound(const_reference value)
		{
			return std::upper_bound(begin(), end(), value, compare_type{});
		}

		ML_NODISCARD inline const_iterator upper_bound(const_reference value) const
		{
			return std::upper_bound(cbegin(), cend(), value, compare_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline iterator find(const_reference value)
		{
			// linear
			if (size() < traits_type::thresh)
			{
				return std::find(begin(), end(), value);
			}
			// binary
			if (auto const it{ equal_range(value) }; it.first != it.second)
			{
				return it.first;
			}
			return end();
		}

		ML_NODISCARD inline const_iterator find(const_reference value) const
		{
			// linear
			if (size() < traits_type::thresh)
			{
				return std::find(cbegin(), cend(), value);
			}
			// binary
			if (auto const it{ equal_range(value) }; it.first != it.second)
			{
				return it.first;
			}
			return cend();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto operator[](size_type const i) -> reference { return m_storage[i]; }

		ML_NODISCARD inline auto operator[](size_type const i) const -> const_reference { return m_storage[i]; }

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

		ML_NODISCARD inline bool operator==(self_type const & other) const
		{
			return (m_storage == other.m_storage);
		}

		ML_NODISCARD inline bool operator!=(self_type const & other) const
		{
			return (m_storage != other.m_storage);
		}

		ML_NODISCARD inline bool operator<(self_type const & other) const
		{
			return (m_storage < other.m_storage);
		}

		ML_NODISCARD inline bool operator>(self_type const & other) const
		{
			return (m_storage > other.m_storage);
		}

		ML_NODISCARD inline bool operator<=(self_type const & other) const
		{
			return (m_storage <= other.m_storage);
		}

		ML_NODISCARD inline bool operator>=(self_type const & other) const
		{
			return (m_storage >= other.m_storage);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		storage_type m_storage;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// FLAT SET - sorted vector of unique elements
	template <class _Ty, class _Pr = std::less<_Ty>, size_t _Thresh = 42
	> struct flat_set : basic_flat_set<basic_flat_set_traits<_Ty, _Pr, false, _Thresh>>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type					= typename flat_set<_Ty, _Pr>;
		using base_type					= typename basic_flat_set<basic_flat_set_traits<_Ty, _Pr, false, _Thresh>>;
		using traits_type				= typename base_type::traits_type;
		using value_type				= typename base_type::value_type;
		using compare_type				= typename base_type::compare_type;
		using allocator_type			= typename base_type::allocator_type;
		using difference_type			= typename base_type::difference_type;
		using size_type					= typename base_type::size_type;

		using storage_type				= typename base_type::storage_type;
		using pointer					= typename base_type::pointer;
		using reference					= typename base_type::reference;
		using const_pointer				= typename base_type::const_pointer;
		using const_reference			= typename base_type::const_reference;
		using iterator					= typename base_type::iterator;
		using const_iterator			= typename base_type::const_iterator;
		using reverse_iterator			= typename base_type::reverse_iterator;
		using const_reverse_iterator	= typename base_type::const_reverse_iterator;
		using initializer_type			= typename base_type::initializer_type;
		using iterator_pair				= typename base_type::iterator_pair;
		using const_iterator_pair		= typename base_type::const_iterator_pair;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		flat_set() noexcept
			: base_type{}
		{
		}

		explicit flat_set(allocator_type const & alloc) noexcept
			: base_type{ alloc }
		{
		}

		flat_set(initializer_type init, allocator_type const & alloc = {})
			: base_type{ init, alloc }
		{
			this->sort();
		}

		template <class It
		> flat_set(It first, It last, allocator_type const & alloc = {})
			: base_type{ first, last, alloc }
		{
			this->sort();
		}

		explicit flat_set(storage_type const & storage, allocator_type const & alloc = {})
			: base_type{ storage, alloc }
		{
			this->sort();
		}

		explicit flat_set(storage_type && storage, allocator_type const & alloc = {}) noexcept
			: base_type{ std::move(storage), alloc }
		{
			this->sort();
		}

		flat_set(self_type const & other, allocator_type const & alloc = {})
			: base_type{ other, alloc }
		{
		}

		flat_set(self_type && other, allocator_type const & alloc = {}) noexcept
			: base_type{ std::move(other), alloc }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			base_type::swap(temp);
			return (*this);
		}

		inline self_type & operator=(self_type && other) noexcept
		{
			base_type::swap(std::move(other));
			return (*this);
		}

		inline self_type & operator=(initializer_type init)
		{
			self_type temp{ init };
			base_type::swap(temp);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline std::pair<iterator, bool> insert(const_reference value)
		{
			if (auto const it{ base_type::equal_range(value) }
			; it.first == it.second)
			{
				return { m_storage.emplace(it.second, value), true };
			}
			else
			{
				return { it.second, false };
			}
		}

		inline std::pair<iterator, bool> insert(value_type && value)
		{
			if (auto const it{ base_type::equal_range(std::move(value)) }
			; it.first == it.second)
			{
				return { m_storage.emplace(it.second, std::move(value)), true };
			}
			else
			{
				return { it.second, false };
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		inline void sort()
		{
			std::sort(base_type::begin(), base_type::end(), compare_type{});

			base_type::erase(
				std::unique(base_type::begin(), base_type::end(), compare_type{}),
				base_type::end()
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// FLAT MULTISET - sorted vector of elements
	template <class _Ty, class _Pr = std::less<_Ty>, size_t _Thresh = 42
	> struct flat_multiset : basic_flat_set<
		basic_flat_set_traits<_Ty, _Pr, true, _Thresh>
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type					= typename flat_multiset<_Ty, _Pr>;
		using base_type					= typename basic_flat_set<basic_flat_set_traits<_Ty, _Pr, true, _Thresh>>;
		using traits_type				= typename base_type::traits_type;
		using value_type				= typename base_type::value_type;
		using compare_type				= typename base_type::compare_type;
		using allocator_type			= typename base_type::allocator_type;
		using difference_type			= typename base_type::difference_type;
		using size_type					= typename base_type::size_type;

		using storage_type				= typename base_type::storage_type;
		using pointer					= typename base_type::pointer;
		using reference					= typename base_type::reference;
		using const_pointer				= typename base_type::const_pointer;
		using const_reference			= typename base_type::const_reference;
		using iterator					= typename base_type::iterator;
		using const_iterator			= typename base_type::const_iterator;
		using reverse_iterator			= typename base_type::reverse_iterator;
		using const_reverse_iterator	= typename base_type::const_reverse_iterator;
		using initializer_type			= typename base_type::initializer_type;
		using iterator_pair				= typename base_type::iterator_pair;
		using const_iterator_pair		= typename base_type::const_iterator_pair;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		flat_multiset() noexcept
			: base_type{}
		{
		}

		explicit flat_multiset(allocator_type const & alloc) noexcept
			: base_type{ alloc }
		{
		}

		flat_multiset(initializer_type init, allocator_type const & alloc = {})
			: base_type{ init, alloc }
		{
			this->sort();
		}

		template <class It
		> flat_multiset(It first, It last, allocator_type const & alloc = {})
			: base_type{ first, last, alloc }
		{
			this->sort();
		}

		explicit flat_multiset(storage_type const & storage, allocator_type const & alloc = {})
			: base_type{ storage, alloc }
		{
			this->sort();
		}

		explicit flat_multiset(storage_type && storage, allocator_type const & alloc = {}) noexcept
			: base_type{ std::move(storage), alloc }
		{
			this->sort();
		}

		flat_multiset(self_type const & other, allocator_type const & alloc = {})
			: base_type{ other, alloc }
		{
		}

		flat_multiset(self_type && other, allocator_type const & alloc = {}) noexcept
			: base_type{ std::move(other), alloc }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			base_type::swap(temp);
			return (*this);
		}

		inline self_type & operator=(self_type && other) noexcept
		{
			base_type::swap(std::move(other));
			return (*this);
		}

		inline self_type & operator=(initializer_type init)
		{
			self_type temp{ init };
			base_type::swap(temp);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator insert(const_reference value)
		{
			return m_storage.emplace(base_type::upper_bound(value), value);
		}

		inline iterator insert(value_type && value)
		{
			return m_storage.emplace(base_type::upper_bound(std::move(value)), std::move(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		inline void sort() noexcept
		{
			std::sort(base_type::begin(), base_type::end(), compare_type{});
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FLAT_SET_HPP_