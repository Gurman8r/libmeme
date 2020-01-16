#ifndef _ML_ARRAY_HPP_
#define _ML_ARRAY_HPP_

#include <libmeme/Core/Alg.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// fixed size array
	template <class T, size_t Size> struct Array
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using self_type			= typename Array<value_type, Size>;
		using storage_type		= typename value_type[Size];
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
			if (Size <= i)
			{
				ML_THROW(std::out_of_range("array subscript out of range"));
			}
			return m_data[i];
		}

		ML_NODISCARD constexpr const_reference operator[](size_type const i) const
		{
			if (Size <= i)
			{
				ML_THROW(std::out_of_range("array subscript out of range"));
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

		ML_NODISCARD constexpr auto max_size() const noexcept -> size_t { return Size; }

		ML_NODISCARD constexpr auto size() const noexcept -> size_t { return Size; }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// zero size array
	template <class T> struct Array<T, 0>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using value_type		= typename T;
		using self_type			= typename Array<value_type, 0>;
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

	template <class T, size_t ... N
	> ML_USING array = Array<T, N...>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t Size
	> inline ML_SERIALIZE(std::ostream & out, array<T, Size> const & value)
	{
		for (auto const & elem : value)
		{
			out << elem << " ";
		}
		return out;
	}

	template <class T, size_t Size
	> inline ML_DESERIALIZE(std::istream & in, array<T, Size> & value)
	{
		for (auto & elem : value)
		{
			if (in.good()) in >> elem;
		}
		return in;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t Size
	> constexpr bool operator==(array<Tx, Size> const & lhs, array<Ty, Size> const & rhs)
	{
		return _ML_ALG equals(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class Tx, class Ty, size_t Size
	> constexpr bool operator!=(array<Tx, Size> const & lhs, array<Ty, Size> const & rhs)
	{
		return !(lhs == rhs);
	}

	template <class Tx, class Ty, size_t Size
	> constexpr bool operator<(array<Tx, Size> const & lhs, array<Ty, Size> const & rhs)
	{
		return _ML_ALG less(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class Tx, class Ty, size_t Size
	> constexpr bool operator<=(array<Tx, Size> const & lhs, array<Ty, Size> const & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}

	template <class Tx, class Ty, size_t Size
	> constexpr bool operator>(array<Tx, Size> const & lhs, array<Ty, Size> const & rhs)
	{
		return !(lhs < rhs) && (lhs != rhs);
	}

	template <class Tx, class Ty, size_t Size
	> constexpr bool operator>=(array<Tx, Size> const & lhs, array<Ty, Size> const & rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_ARRAY_HPP_