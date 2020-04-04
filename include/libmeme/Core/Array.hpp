#ifndef _ML_ARRAY_HPP_
#define _ML_ARRAY_HPP_

#include <libmeme/Core/Utility.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// FIXED SIZE ARRAY
namespace ml::ds
{
	template <class _T, size_t _N> struct array
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type				= typename _T;
		using self_type					= typename array<value_type, _N>;
		using storage_type				= typename value_type[_N];
		using size_type					= typename size_t;
		using difference_type			= typename ptrdiff_t;
		using pointer					= typename value_type *;
		using reference					= typename value_type &;
		using const_pointer				= typename value_type const *;
		using const_reference			= typename value_type const &;
		using iterator					= typename pointer;
		using const_iterator			= typename const_pointer;
		using reverse_iterator			= typename std::reverse_iterator<iterator>;
		using const_reverse_iterator	= typename std::reverse_iterator<const_iterator>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		storage_type m_data; // aggregate initializer

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr operator pointer() noexcept { return m_data; }

		ML_NODISCARD constexpr operator const_pointer() const noexcept { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto operator*() & noexcept { return (*m_data); }

		ML_NODISCARD constexpr auto operator*() const & noexcept { return (*m_data); }

		ML_NODISCARD constexpr auto operator*() && noexcept { return std::move(*m_data); }

		ML_NODISCARD constexpr auto operator*() const && noexcept { return std::move(*m_data); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto at(size_t const i) & noexcept { return m_data[i]; }

		ML_NODISCARD constexpr auto at(size_t const i) const & noexcept { return m_data[i]; }

		ML_NODISCARD constexpr auto at(size_t const i) && noexcept { return std::move(m_data[i]); }

		ML_NODISCARD constexpr auto at(size_t const i) const && noexcept { return std::move(m_data[i]); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto back() & noexcept -> reference { return m_data[_N - 1]; }
		
		ML_NODISCARD constexpr auto back() const & noexcept -> const_reference { return m_data[_N - 1]; }
		
		ML_NODISCARD constexpr auto data() noexcept -> pointer { return m_data; }
		
		ML_NODISCARD constexpr auto data() const noexcept -> const_pointer { return m_data; }
		
		ML_NODISCARD constexpr bool empty() const noexcept { return false; }
		
		ML_NODISCARD constexpr auto front() & noexcept -> reference { return m_data[0]; }
		
		ML_NODISCARD constexpr auto front() const & noexcept -> const_reference { return m_data[0]; }

		ML_NODISCARD constexpr auto max_size() const noexcept -> size_t { return _N; }

		ML_NODISCARD constexpr auto size() const noexcept -> size_t { return _N; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto begin() noexcept -> iterator { return m_data; }

		ML_NODISCARD constexpr auto begin() const noexcept -> const_iterator { return m_data; }

		ML_NODISCARD constexpr auto end() noexcept -> iterator { return m_data + _N; }

		ML_NODISCARD constexpr auto end() const noexcept -> const_iterator { return m_data + _N; }

		ML_NODISCARD constexpr auto cbegin() const noexcept -> const_iterator { return begin(); }

		ML_NODISCARD constexpr auto cend() const noexcept -> const_iterator { return end(); }

		ML_NODISCARD constexpr auto crbegin() const noexcept -> const_reverse_iterator { return rbegin(); }

		ML_NODISCARD constexpr auto crend() const noexcept -> const_reverse_iterator { return rend(); }

		ML_NODISCARD constexpr auto rbegin() noexcept -> reverse_iterator { return std::make_reverse_iterator(end()); }

		ML_NODISCARD constexpr auto rbegin() const noexcept -> const_reverse_iterator { return std::make_reverse_iterator(cend()); }

		ML_NODISCARD constexpr auto rend() noexcept -> reverse_iterator { return std::make_reverse_iterator(begin()); }

		ML_NODISCARD constexpr auto rend() const noexcept -> const_reverse_iterator { return std::make_reverse_iterator(cbegin()); }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// ZERO SIZE ARRAY
namespace ml::ds
{
	template <class _T> struct array<_T, 0>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using value_type				= typename _T;
		using self_type					= typename array<value_type, 0>;
		using storage_type				= typename value_type[1];
		using size_type					= typename size_t;
		using difference_type			= typename ptrdiff_t;
		using pointer					= typename value_type *;
		using reference					= typename value_type &;
		using const_pointer				= typename value_type const *;
		using const_reference			= typename value_type const &;
		using iterator					= typename pointer;
		using const_iterator			= typename const_pointer;
		using reverse_iterator			= typename std::reverse_iterator<iterator>;
		using const_reverse_iterator	= typename std::reverse_iterator<const_iterator>;
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		storage_type m_data; // aggregate initializer
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr operator pointer() noexcept { return m_data; }

		ML_NODISCARD constexpr operator const_pointer() const noexcept { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto operator*() & noexcept { return (*m_data); }

		ML_NODISCARD constexpr auto operator*() const & noexcept { return (*m_data); }

		ML_NODISCARD constexpr auto operator*() && noexcept { return std::move(*m_data); }

		ML_NODISCARD constexpr auto operator*() const && noexcept { return std::move(*m_data); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto at(size_t) & noexcept { return m_data[0]; }
		
		ML_NODISCARD constexpr auto at(size_t) const & noexcept { return m_data[0]; }

		ML_NODISCARD constexpr auto at(size_t) && noexcept { return std::move(m_data[0]); }

		ML_NODISCARD constexpr auto at(size_t) const && noexcept { return std::move(m_data[0]); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto back() & noexcept -> reference { return m_data[0]; }
		
		ML_NODISCARD constexpr auto back() const & noexcept -> const_reference { return m_data[0]; }
		
		ML_NODISCARD constexpr auto data() noexcept -> pointer { return &m_data[0]; }
		
		ML_NODISCARD constexpr auto data() const noexcept -> const_pointer { return &m_data[0]; }

		ML_NODISCARD constexpr bool empty() const noexcept { return true; }
		
		ML_NODISCARD constexpr auto front() & noexcept -> reference { return m_data[0]; }
		
		ML_NODISCARD constexpr auto front() const & noexcept -> const_reference { return m_data[0]; }

		ML_NODISCARD constexpr auto max_size() const noexcept -> size_t { return 0; }

		ML_NODISCARD constexpr auto size() const noexcept -> size_t { return 0; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		ML_NODISCARD constexpr auto begin() noexcept -> iterator { return &m_data[0]; }

		ML_NODISCARD constexpr auto begin() const noexcept -> const_iterator { return &m_data[0]; }

		ML_NODISCARD constexpr auto cbegin() const noexcept -> const_iterator { return &m_data[0]; }

		ML_NODISCARD constexpr auto cend() const noexcept -> const_iterator { return &m_data[0]; }

		ML_NODISCARD constexpr auto crbegin() const noexcept -> const_reverse_iterator { return &m_data[0]; }

		ML_NODISCARD constexpr auto crend() const noexcept -> const_reverse_iterator { return &m_data[0]; }
		
		ML_NODISCARD constexpr auto end() noexcept -> iterator { return &m_data[0]; }

		ML_NODISCARD constexpr auto end() const noexcept -> const_iterator { return &m_data[0]; }

		ML_NODISCARD constexpr auto rbegin() noexcept -> reverse_iterator { return &m_data[0]; }

		ML_NODISCARD constexpr auto rbegin() const noexcept -> const_reverse_iterator { return &m_data[0]; }

		ML_NODISCARD constexpr auto rend() noexcept -> reverse_iterator { return &m_data[0]; }

		ML_NODISCARD constexpr auto rend() const noexcept -> const_reverse_iterator { return &m_data[0]; }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// OPERATORS
namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t N
	> constexpr bool operator==(array<Tx, N> const & lhs, array<Ty, N> const & rhs)
	{
		return util::equal_to(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class Tx, class Ty, size_t N
	> constexpr bool operator!=(array<Tx, N> const & lhs, array<Ty, N> const & rhs)
	{
		return !(lhs == rhs);
	}

	template <class Tx, class Ty, size_t N
	> constexpr bool operator<(array<Tx, N> const & lhs, array<Ty, N> const & rhs)
	{
		return util::less(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class Tx, class Ty, size_t N
	> constexpr bool operator<=(array<Tx, N> const & lhs, array<Ty, N> const & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <class Tx, class Ty, size_t N
	> constexpr bool operator>(array<Tx, N> const & lhs, array<Ty, N> const & rhs)
	{
		return !(lhs < rhs) && (lhs != rhs);
	}

	template <class Tx, class Ty, size_t N
	> constexpr bool operator>=(array<Tx, N> const & lhs, array<Ty, N> const & rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// TUPLE INTERFACE
namespace std
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t I, class T, size_t N
	> ML_NODISCARD constexpr T & get(_ML ds::array<T, N> & value) noexcept
	{
		static_assert(I < N, "array index out of bounds");
		return value.at(I);
	}

	template <size_t I, class T, size_t N
	> ML_NODISCARD constexpr T const & get(_ML ds::array<T, N> const & value) noexcept
	{
		static_assert(I < N, "array index out of bounds");
		return value.at(I);
	}

	template <size_t I, class T, size_t N
	> ML_NODISCARD constexpr T && get(_ML ds::array<T, N> && value) noexcept
	{
		static_assert(I < N, "array index out of bounds");
		return std::move(value.at(I));
	}

	template <size_t I, class T, size_t N
	> ML_NODISCARD constexpr T const && get(_ML ds::array<T, N> const && value) noexcept
	{
		static_assert(I < N, "array index out of bounds");
		return std::move(value.at(I));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// JSON INTERFACE
namespace ml::ds
{
	template <class T, size_t N
	> static void to_json(json & j, array<T, N> const & value)
	{
		// SPAGHET: json wants iterators
		j = *reinterpret_cast<std::array<T, N> const *>(&value);
	}

	template <class T, size_t N
	> static void from_json(json const & j, array<T, N> & value)
	{
		// SPAGHET: json wants iterators
		j.get_to(*reinterpret_cast<std::array<T, N> *>(&value));
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_ARRAY_HPP_