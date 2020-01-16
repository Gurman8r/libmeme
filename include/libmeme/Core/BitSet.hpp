#ifndef _ML_BIT_SET_HPP_
#define _ML_BIT_SET_HPP_

#include <libmeme/Core/Array.hpp>

#define ML_BITREAD(value, bit)		((value >> bit) & 1)
#define ML_BITSET(value, bit)		(value |= (1 << bit))
#define ML_BITCLEAR(value, bit)		(value &= ~(1 << bit))
#define ML_BITWRITE(value, bit, b)	(b ? ML_BITSET(value, bit) : ML_BITCLEAR(value, bit))

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> struct BitSet final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { Size = sizeof(T) * 8 };

		using value_type		= typename T;
		using self_type			= typename BitSet<value_type>;
		using storage_type		= typename array<bool, Size>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename value_type const *;
		using const_reference	= typename value_type const &;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr BitSet(const value_type value) noexcept
			: m_value{ value }
		{
		}

		constexpr BitSet(self_type const & copy) noexcept
			: self_type{ copy.m_value }
		{
		}

		constexpr BitSet(storage_type const & value)
			: self_type{ from_bits<storage_type, T, value.size()>(value) }
		{
		}

		template <
			class U, size_t N
		> constexpr BitSet(const U(&value)[N])
			: self_type{ from_bits<const U(&)[N], U, N>(value) }
		{
		}

		constexpr BitSet() noexcept : self_type{ 0 } {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static constexpr auto size() noexcept
		{
			return self_type::Size;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto bits() const noexcept -> storage_type { return to_bits(*this); }

		ML_NODISCARD constexpr auto data() const noexcept -> const_reference { return m_value; }

		ML_NODISCARD constexpr auto data() noexcept -> reference { return m_value; }

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

		ML_NODISCARD constexpr bool read(size_t i) const noexcept
		{
			return ML_BITREAD(m_value, i);
		}

		constexpr self_type & clear(size_t i)
		{
			ML_BITCLEAR(m_value, i); return (*this);
		}

		constexpr self_type & set(size_t i)
		{
			ML_BITSET(m_value, i); return (*this);
		}

		constexpr self_type & write(size_t i, bool value)
		{
			ML_BITWRITE(m_value, i, value); return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I
		> ML_NODISCARD constexpr bool read() const noexcept
		{
			return this->read(I);
		}

		template <size_t I
		> constexpr self_type & clear()
		{
			return this->clear(I);
		}

		template <size_t I
		> constexpr self_type & set()
		{
			return this->set(I);
		}

		template <size_t I
		> constexpr self_type & write(bool value)
		{
			return this->write(I, value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class In, class U, size_t N
		> ML_NODISCARD static constexpr value_type from_bits(In const & value)
		{
			value_type temp{ 0 };
			for (size_t i = 0; i < N; ++i)
			{
				ML_BITWRITE(temp, i, value[i]);
			}
			return temp;
		}

		ML_NODISCARD static constexpr storage_type to_bits(self_type const & value)
		{
			storage_type temp{ 0 };
			for (size_t i = 0; i < Size; ++i)
			{
				temp[i] = ML_BITREAD(value.m_value, i);
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