#ifndef _ML_BIT_SET_HPP_
#define _ML_BIT_SET_HPP_

#include <libmeme/Core/Array.hpp>

#define ML_bitread(value, index)		((value >> index) & 1)
#define ML_bitset(value, index)			(value |= (1 << index))
#define ML_bitclear(value, index)		(value &= ~(1 << index))
#define ML_bitwrite(value, index, b)	(b ? ML_bitset(value, index) : ML_bitclear(value, index))

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> struct BitSet final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { Size = sizeof(T) * 8 };

		using value_type		= typename T;
		using self_type			= typename BitSet<value_type>;
		using array_type		= typename array<bool, Size>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename value_type const *;
		using const_reference	= typename value_type const &;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr BitSet(value_type const value) noexcept : m_value{ value } {}

		constexpr BitSet() noexcept : self_type{ 0 } {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr BitSet(self_type const & other)
			: self_type{ other.m_value }
		{
		}

		constexpr BitSet(self_type && other) noexcept
			: self_type{ std::move(other.m_value) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr BitSet(array_type const & value)
			: self_type{ from_bits<array_type, T, value.size()>(value) }
		{
		}

		template <class U, size_t N
		> constexpr BitSet(const U(&value)[N])
			: self_type{ from_bits<const U(&)[N], U, N>(value) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			swap(temp);
			return (*this);
		}

		constexpr self_type & operator=(self_type && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		constexpr void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				util::swap(m_value, other.m_value);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr operator const_reference() const noexcept
		{
			return m_value;
		}

		ML_NODISCARD constexpr bool operator[](size_t i) const noexcept
		{
			return this->read(i);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr array_type bits() const noexcept
		{
			return self_type::to_bits(*this);
		}

		ML_NODISCARD constexpr const_reference data() const noexcept
		{
			return m_value;
		}

		ML_NODISCARD constexpr reference data() noexcept
		{
			return m_value;
		}

		ML_NODISCARD constexpr size_t size() const noexcept
		{
			return self_type::Size;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr bool read(size_t index) const noexcept
		{
			return ML_bitread(m_value, index);
		}

		constexpr self_type & clear(size_t index)
		{
			ML_bitclear(m_value, index);
			return (*this);
		}

		constexpr self_type & set(size_t index)
		{
			ML_bitset(m_value, index);
			return (*this);
		}

		constexpr self_type & write(size_t index, bool value)
		{
			ML_bitwrite(m_value, index, value);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t Index
		> ML_NODISCARD constexpr bool read() const noexcept
		{
			return this->read(Index);
		}

		template <size_t Index
		> constexpr self_type & clear()
		{
			return this->clear(Index);
		}

		template <size_t Index
		> constexpr self_type & set()
		{
			return this->set(Index);
		}

		template <size_t Index
		> constexpr self_type & write(bool value)
		{
			return this->write(Index, value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Arr, class U, size_t N
		> ML_NODISCARD static constexpr value_type from_bits(Arr const & value)
		{
			value_type temp{ 0 };
			for (size_t i = 0; i < N; ++i)
			{
				ML_bitwrite(temp, i, value[i]);
			}
			return temp;
		}

		ML_NODISCARD static constexpr array_type to_bits(self_type const & value)
		{
			array_type temp{ 0 };
			for (size_t i = 0; i < Size; ++i)
			{
				temp[i] = ML_bitread(value.m_value, i);
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		value_type m_value;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	ML_USING mask8_t = typename BitSet<uint8_t>;
	ML_USING mask16_t = typename BitSet<uint16_t>;
	ML_USING mask32_t = typename BitSet<uint32_t>;
	ML_USING mask64_t = typename BitSet<uint64_t>;

	/* * * * * * * * * * * * * * * * * * * * */

	template <class T>
	inline ML_SERIALIZE(std::ostream & out, const BitSet<T> & value)
	{
		return out << value.bits();
	}

	template <class T>
	inline ML_DESERIALIZE(std::istream & in, BitSet<T> & value)
	{
		return in.good() ? (in >> value.data()) : in;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BIT_SET_HPP_