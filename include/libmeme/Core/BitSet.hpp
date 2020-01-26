#ifndef _ML_BIT_SET_HPP_
#define _ML_BIT_SET_HPP_

#include <libmeme/Core/Utility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t _Size = 64
	> struct bit_set
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static_assert(_Size <= 64);

		using value_type = typename uint64_t;
		using array_type = typename ds::array<bool, _Size>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> constexpr bit_set(T const value) noexcept
			: m_value{ static_cast<uint64_t>(value) }
		{
		}

		constexpr bit_set() noexcept
			: bit_set{ 0 }
		{
		}

		constexpr bit_set(bit_set const & other)
			: bit_set{ other.m_value }
		{
		}

		constexpr bit_set(bit_set && other) noexcept
			: bit_set{ std::move(other.m_value) }
		{
		}

		constexpr bit_set(array_type const & arr) noexcept
			: bit_set{}
		{
			for (auto it = arr.begin(); it != arr.end(); ++it)
			{
				write(std::distance(str.begin(), it), *it);
			}
		}

		constexpr bit_set(std::string_view const & str) noexcept
			: bit_set{}
		{
			for (auto it = str.begin(); it != str.end(); ++it)
			{
				write(std::distance(str.begin(), it), (*it) != '0');
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		constexpr bit_set & operator=(bit_set const & other)
		{
			bit_set temp{ other };
			swap(temp);
			return (*this);
		}

		constexpr bit_set & operator=(bit_set && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> constexpr bit_set & operator=(T const value) noexcept
		{
			bit_set temp{ value };
			swap(temp);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator uint64_t const &() const noexcept
		{
			return m_value;
		}

		constexpr bool operator[](size_t const i) const noexcept
		{
			return read(i);
		}

		constexpr array_type arr() const noexcept
		{
			array_type temp{};
			for (size_t i = 0; i < _Size; ++i)
				temp[i] = read(i);
			return temp;
		}

		constexpr bit_set & clear() noexcept
		{
			m_value = 0;
			return (*this);
		}

		constexpr bit_set & clear(size_t const i) noexcept
		{
			if (i < size()) m_value &= ~(1ULL << static_cast<uint64_t>(i));
			return (*this);
		}

		constexpr bool read(size_t const i) const noexcept
		{
			return (i < size()) ? ((m_value >> static_cast<uint64_t>(i)) & 1ULL) : false;
		}

		constexpr bit_set & set(size_t const i) noexcept
		{
			if (i < size()) m_value |= (1ULL << static_cast<uint64_t>(i));
			return (*this);
		}

		constexpr size_t size() const noexcept
		{
			return _Size;
		}

		constexpr void swap(bit_set & other) noexcept
		{
			if (this != std::addressof(other))
			{
				util::swap(m_value, other.m_value);
			}
		}

		constexpr bit_set & write(size_t const i, bool b) noexcept
		{
			return b ? set(i) : clear(i);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> ML_NODISCARD constexpr bool operator==(T const & other) const
		{
			return !(*this < other) && !(other < *this);
		}

		template <class T> ML_NODISCARD constexpr bool operator!=(T const & other) const
		{
			return !(*this == other);
		}

		template <class T> ML_NODISCARD constexpr bool operator<(T const & other) const
		{
			return (m_value < static_cast<uint64_t>(other));
		}

		template <class T> ML_NODISCARD constexpr bool operator>(T const & other) const
		{
			return !(*this < other);
		}

		template <class T> ML_NODISCARD constexpr bool operator<=(T const & other) const
		{
			return (*this < other) || (*this == other);
		}

		template <class T> ML_NODISCARD constexpr bool operator>=(T const & other) const
		{
			return (*this > other) || (*this == other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		uint64_t m_value;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


	template <size_t N> inline ML_SERIALIZE(std::ostream & out, bit_set<N> const & value)
	{
		for (size_t i = 0; i < N; ++i)
		{
			out << value.read(i);
		}
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BIT_SET_HPP_