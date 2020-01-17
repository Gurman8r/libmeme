#ifndef _ML_ARRAY_HPP_
#define _ML_ARRAY_HPP_

#include <libmeme/Core/Utility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// fixed size array
	template <class _Ty,	// value type
		size_t _Size		// element count
	> struct array
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

		ML_NODISCARD constexpr reference operator[](size_type const i)
		{
			if (_Size <= i)
			{
				ML_THROW std::out_of_range("ml::array subscript out of range");
			}
			return m_data[i];
		}

		ML_NODISCARD constexpr const_reference operator[](size_type const i) const
		{
			if (_Size <= i)
			{
				ML_THROW std::out_of_range("ml::array subscript out of range");
			}
			return m_data[i];
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto at(size_type const i) -> reference { return operator[](i); }

		ML_NODISCARD constexpr auto at(size_type const i) const -> const_reference { return operator[](i); }
		
		ML_NODISCARD constexpr auto back() noexcept -> reference { return (*end()); }
		
		ML_NODISCARD constexpr auto back() const noexcept -> const_reference { return (*cend()); }
		
		ML_NODISCARD constexpr auto begin() noexcept -> iterator { return data(); }
		
		ML_NODISCARD constexpr auto begin() const noexcept -> const_iterator { return data(); }
		
		ML_NODISCARD constexpr auto cbegin() const noexcept -> const_iterator { return begin(); }
		
		ML_NODISCARD constexpr auto cend() const noexcept -> const_iterator { return end(); }
		
		ML_NODISCARD constexpr auto data() noexcept -> pointer { return m_data; }
		
		ML_NODISCARD constexpr auto data() const noexcept -> const_pointer { return m_data; }
		
		ML_NODISCARD constexpr bool empty() const noexcept { return false; }
		
		ML_NODISCARD constexpr auto end() noexcept -> iterator { return data() + size(); }
		
		ML_NODISCARD constexpr auto end() const noexcept -> const_iterator { return data() + size(); }
		
		ML_NODISCARD constexpr auto front() noexcept -> reference { return (*begin()); }
		
		ML_NODISCARD constexpr auto front() const noexcept -> const_reference { return (*cbegin()); }

		ML_NODISCARD constexpr auto max_size() const noexcept -> size_t { return _Size; }

		ML_NODISCARD constexpr auto size() const noexcept -> size_t { return _Size; }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// zero size array
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

		ML_NODISCARD constexpr reference operator[](size_t const) { return m_data[0]; }

		ML_NODISCARD constexpr const_reference operator[](size_t const) const { m_data[0]; }

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t N
	> constexpr bool operator==(array<Tx, N> const & lhs, array<Ty, N> const & rhs)
	{
		return util::equals(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
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

#endif // !_ML_ARRAY_HPP_