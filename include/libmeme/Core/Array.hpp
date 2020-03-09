#ifndef _ML_ARRAY_HPP_
#define _ML_ARRAY_HPP_

#include <libmeme/Core/Utility.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// FIXED SIZE ARRAY
namespace ml::ds
{
	template <class _Ty, size_t _Size> struct array
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename _Ty;
		using self_type			= typename array<value_type, _Size>;
		using storage_type		= typename value_type[_Size];
		using size_type			= typename size_t;
		using difference_type	= typename ptrdiff_t;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename value_type const *;
		using const_reference	= typename value_type const &;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		storage_type m_data; // aggregate initializer

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr operator pointer() noexcept { return m_data; }

		ML_NODISCARD constexpr operator const_pointer() const noexcept { return m_data; }

		ML_NODISCARD constexpr auto operator*() noexcept -> reference { return (*m_data); }

		ML_NODISCARD constexpr auto operator*() const noexcept -> const_reference { return (*m_data); }

		ML_NODISCARD constexpr auto at(size_type const i) noexcept -> reference { return m_data[i]; }

		ML_NODISCARD constexpr auto at(size_type const i) const noexcept -> const_reference { return m_data[i]; }

		ML_NODISCARD constexpr auto back() noexcept -> reference { return m_data[_Size - 1]; }
		
		ML_NODISCARD constexpr auto back() const noexcept -> const_reference { return m_data[_Size - 1]; }
		
		ML_NODISCARD constexpr auto begin() noexcept -> iterator { return m_data; }
		
		ML_NODISCARD constexpr auto begin() const noexcept -> const_iterator { return m_data; }
		
		ML_NODISCARD constexpr auto cbegin() const noexcept -> const_iterator { return m_data; }
		
		ML_NODISCARD constexpr auto cend() const noexcept -> const_iterator { return m_data + _Size; }
		
		ML_NODISCARD constexpr auto data() noexcept -> pointer { return m_data; }
		
		ML_NODISCARD constexpr auto data() const noexcept -> const_pointer { return m_data; }
		
		ML_NODISCARD constexpr bool empty() const noexcept { return false; }
		
		ML_NODISCARD constexpr auto end() noexcept -> iterator { return m_data + _Size; }
		
		ML_NODISCARD constexpr auto end() const noexcept -> const_iterator { return m_data + _Size; }
		
		ML_NODISCARD constexpr auto front() noexcept -> reference { return m_data[0]; }
		
		ML_NODISCARD constexpr auto front() const noexcept -> const_reference { return m_data[0]; }

		ML_NODISCARD constexpr auto max_size() const noexcept -> size_t { return _Size; }

		ML_NODISCARD constexpr auto size() const noexcept -> size_t { return _Size; }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// ZERO SIZE ARRAY
namespace ml::ds
{
	template <class _Ty> struct array<_Ty, 0>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using value_type		= typename _Ty;
		using self_type			= typename array<value_type, 0>;
		using storage_type		= typename value_type[1];
		using size_type			= typename size_t;
		using difference_type	= typename ptrdiff_t;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename value_type const *;
		using const_reference	= typename value_type const &;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		storage_type m_data; // aggregate initializer
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr operator pointer() noexcept { return m_data; }

		ML_NODISCARD constexpr operator const_pointer() const noexcept { return m_data; }

		ML_NODISCARD constexpr auto operator*() noexcept -> reference { return (*m_data); }

		ML_NODISCARD constexpr auto operator*() const noexcept -> const_reference { return (*m_data); }

		ML_NODISCARD constexpr auto at(size_t const) -> reference { return m_data[0]; }
		
		ML_NODISCARD constexpr auto at(size_t const) const -> const_reference { return m_data[0]; }

		ML_NODISCARD constexpr auto back() noexcept -> reference { return m_data[0]; }
		
		ML_NODISCARD constexpr auto back() const noexcept -> const_reference { return m_data[0]; }
		
		ML_NODISCARD constexpr auto begin() noexcept -> iterator { return &m_data[0]; }
		
		ML_NODISCARD constexpr auto begin() const noexcept -> const_iterator { return &m_data[0]; }
		
		ML_NODISCARD constexpr auto cbegin() const noexcept -> const_iterator { return &m_data[0]; }
		
		ML_NODISCARD constexpr auto cend() const noexcept -> const_iterator { return &m_data[0]; }
		
		ML_NODISCARD constexpr auto data() noexcept -> pointer { return &m_data[0]; }
		
		ML_NODISCARD constexpr auto data() const noexcept -> const_pointer { return &m_data[0]; }

		ML_NODISCARD constexpr bool empty() const noexcept { return true; }
		
		ML_NODISCARD constexpr auto end() noexcept -> iterator { return &m_data[0]; }
		
		ML_NODISCARD constexpr auto end() const noexcept -> const_iterator { return &m_data[0]; }
		
		ML_NODISCARD constexpr auto front() noexcept -> reference { return m_data[0]; }
		
		ML_NODISCARD constexpr auto front() const noexcept -> const_reference { return m_data[0]; }

		ML_NODISCARD constexpr auto max_size() const noexcept -> size_t { return 0; }

		ML_NODISCARD constexpr auto size() const noexcept -> size_t { return 0; }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// OPERATORS
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t N
	> constexpr bool operator==(ds::array<Tx, N> const & lhs, ds::array<Ty, N> const & rhs)
	{
		return util::equal_to(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class Tx, class Ty, size_t N
	> constexpr bool operator!=(ds::array<Tx, N> const & lhs, ds::array<Ty, N> const & rhs)
	{
		return !(lhs == rhs);
	}

	template <class Tx, class Ty, size_t N
	> constexpr bool operator<(ds::array<Tx, N> const & lhs, ds::array<Ty, N> const & rhs)
	{
		return util::less(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class Tx, class Ty, size_t N
	> constexpr bool operator<=(ds::array<Tx, N> const & lhs, ds::array<Ty, N> const & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <class Tx, class Ty, size_t N
	> constexpr bool operator>(ds::array<Tx, N> const & lhs, ds::array<Ty, N> const & rhs)
	{
		return !(lhs < rhs) && (lhs != rhs);
	}

	template <class Tx, class Ty, size_t N
	> constexpr bool operator>=(ds::array<Tx, N> const & lhs, ds::array<Ty, N> const & rhs)
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
		return value.m_data[I];
	}

	template <size_t I, class T, size_t N
	> ML_NODISCARD constexpr T const & get(const _ML ds::array<T, N> & value) noexcept
	{
		static_assert(I < N, "array index out of bounds");
		return value.m_data[I];
	}

	template <size_t I, class T, size_t N
	> ML_NODISCARD constexpr T && get(_ML ds::array<T, N> && value) noexcept
	{
		static_assert(I < N, "array index out of bounds");
		return _STD move(value.m_data[I]);
	}

	template <size_t I, class T, size_t N
	> ML_NODISCARD constexpr T const && get(const _ML ds::array<T, N> && value) noexcept
	{
		static_assert(I < N, "array index out of bounds");
		return _STD move(value.m_data[I]);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// JSON INTERFACE
namespace ml::ds
{
	template <class T, size_t N
	> static void from_json(json const & j, array<T, N> & value)
	{
		for (size_t i = 0; i < N; ++i)
		{
			j.at(i).get_to(value[i]);
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_ARRAY_HPP_