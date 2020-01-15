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
		using array_type		= typename array<bool, Size>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename value_type const *;
		using const_reference	= typename value_type const &;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr BitSet() noexcept
			: m_data{ 0 }
		{
		}

		constexpr BitSet(value_type const value) noexcept
			: m_data{ value }
		{
		}

		constexpr BitSet(array_type const & value) noexcept
			: m_data{ from_bits<array_type, T, value.size()>(value) }
		{
		}

		template <class U, size_t N
		> constexpr BitSet(const U(&value)[N]) noexcept
			: m_data{ from_bits<const U(&)[N], U, N>(value) }
		{
		}

		constexpr BitSet(self_type const & other)
			: m_data{ other.m_data }
		{
		}

		constexpr BitSet(self_type && other) noexcept
			: m_data{ std::move(other.m_data) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline self_type & operator=(self_type const & other)
		{
			self_type temp{ other };
			this->swap(temp);
			return (*this);
		}

		inline self_type & operator=(self_type && other) noexcept
		{
			this->swap(std::move(other));
			return (*this);
		}

		constexpr void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				alg::swap(m_data, other.m_data);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr auto size() { return self_type::Size; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto bits() const -> array_type { return to_bits(*this); }

		constexpr auto data() const -> const_reference { return m_data; }

		constexpr auto data() -> reference { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator const_reference() const { return m_data; }

		constexpr bool operator[](size_t i) const { return this->read(i); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr bool read(size_t i) const { return ML_BITREAD(m_data, i); }

		constexpr self_type & clear(size_t i) { ML_BITCLEAR(m_data, i); return (*this); }

		constexpr self_type & set(size_t i) { ML_BITSET(m_data, i); return (*this); }

		constexpr self_type & write(size_t i, bool value) { ML_BITWRITE(m_data, i, value); return (*this); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <size_t I> constexpr bool read() const { return this->read(I); }

		template <size_t I> constexpr self_type & clear() { return this->clear(I); }

		template <size_t I> constexpr self_type & set() { return this->set(I); }

		template <size_t I> constexpr self_type & write(bool value) { return this->write(I, value); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class In, class U, size_t N
		> static constexpr value_type from_bits(In const & value)
		{
			value_type temp{ 0 };
			for (size_t i = 0; i < N; i++)
			{
				ML_BITWRITE(temp, i, value[i]);
			}
			return temp;
		}

		static constexpr array_type to_bits(self_type const & value)
		{
			array_type temp{ 0 };
			for (size_t i = 0; i < Size; i++)
			{
				temp[i] = ML_BITREAD(value.m_data, i);
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		value_type m_data;

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

#endif // !_ML_BIT_SET