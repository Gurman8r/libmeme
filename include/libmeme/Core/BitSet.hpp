#ifndef _ML_BIT_SET_HPP_
#define _ML_BIT_SET_HPP_

#include <libmeme/Core/Array.hpp>

namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t BitCount = 64
	> struct bitset
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type = typename bitset<BitCount>;

		using value_type = typename std::conditional_t<
			BitCount <= sizeof(uint32_t) * 8,
			uint32_t,
			uint64_t
		>;

		static constexpr ptrdiff_t bitsperword{ sizeof(value_type) * 8 };
		
		static constexpr ptrdiff_t words{ (BitCount - 1) / bitsperword };

		using storage_type = typename array<value_type, words + 1>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr bitset() noexcept : m_words{ 0 } {}

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> constexpr bitset(T const value) noexcept
			: m_words{ static_cast<uint64_t>(value) }
		{
		}

		constexpr bitset(self_type const & other)
			: m_words{ other.m_words }
		{
		}

		constexpr bitset(self_type && other) noexcept
			: m_words{ std::move(other.m_words) }
		{
		}

		template <size_t N
		> constexpr bitset(bitset<N> const & other)
			: m_words{ other.m_words }
		{
		}

		template <size_t N
		> constexpr bitset(bitset<N> && other) noexcept
			: m_words{ std::move(other.m_words) }
		{
		}

		constexpr bitset(array<bool, BitCount> const & arr) noexcept
			: m_words{}
		{
			for (auto it = arr.begin(); it != arr.end(); ++it)
			{
				write(std::distance(str.begin(), it), *it);
			}
		}

		constexpr bitset(std::string_view const & str) noexcept
			: self_type{}
		{
			for (auto it = str.begin(); it != str.end(); ++it)
			{
				write(std::distance(str.begin(), it), (*it) != '0');
			}
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

		template <class T, class = std::enable_if_t<
			std::is_integral_v<T>
		>> constexpr bitset & operator=(T const value) noexcept
		{
			bitset temp{ value };
			swap(temp);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr operator uint64_t const & () const noexcept
		{
			return m_words;
		}

		constexpr bool operator[](size_t const i) const noexcept
		{
			return read(i);
		}

		constexpr array<bool, BitCount> arr() const noexcept
		{
			array<bool, BitCount> temp{};
			for (size_t i = 0; i < BitCount; ++i)
				temp[i] = read(i);
			return temp;
		}

		template <size_t I> constexpr self_type & clear() noexcept
		{
			static_assert(I < BitCount, "bitset subscript out of range");
			return clear(I);
		}

		constexpr self_type & clear(size_t const i) noexcept
		{
			m_words[i / bitsperword] &= ~(value_type{ 1 } << i % bitsperword);
			return (*this);
		}

		template <size_t I> constexpr bool read() noexcept
		{
			static_assert(I < BitCount, "bitset subscript out of range");
			return read(I);
		}

		constexpr bool read(size_t const i) const noexcept
		{
			return m_words[i / bitsperword] & (value_type{ 1 } << i % bitsperword);
		}

		constexpr self_type & reset() noexcept
		{
			m_words = { 0 };
			return (*this);
		}

		template <size_t I> constexpr self_type & set() noexcept
		{
			static_assert(I < BitCount, "bitset subscript out of range");
			return set(I);
		}

		constexpr self_type & set(size_t const i) noexcept
		{
			m_words[i / bitsperword] |= (value_type{ 1 } << i % bitsperword);
			return (*this);
		}

		constexpr size_t size() const noexcept
		{
			return BitCount;
		}

		constexpr void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				util::swap(m_words, other.m_words);
			}
		}

		template <size_t I> constexpr self_type & write(bool b) noexcept
		{
			static_assert(I < BitCount, "bitset subscript out of range");
			return write(I, b);
		}

		constexpr self_type & write(size_t const i, bool b) noexcept
		{
			return b ? set(i) : clear(i);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> constexpr self_type & operator&=(T other)
		{
			if constexpr (std::is_same_v<T, self_type>)
			{
				size_t i{};
				for (auto & elem : m_words) { elem &= other.m_words[i++]; }
				return (*this);
			}
			else
			{
				return (*this) &= self_type{ other };
			}
		}

		template <class T> constexpr self_type operator&(T other) const
		{
			self_type temp{ other };
			return temp &= (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> constexpr self_type & operator|=(T other)
		{
			if constexpr (std::is_same_v<T, self_type>)
			{
				size_t i{};
				for (auto & elem : m_words) { elem |= other.m_words[i++]; }
				return (*this);
			}
			else
			{
				return (*this) |= self_type{ other };
			}
		}

		template <class T> constexpr self_type operator|(T other) const
		{
			self_type temp{ other };
			return temp |= (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> ML_NODISCARD constexpr bool operator==(T other) const
		{
			if constexpr (std::is_same_v<T, self_type>)
			{
				return m_words == other.m_words;
			}
			else
			{
				return (*this) == self_type{ other };
			}
		}

		template <class T> ML_NODISCARD constexpr bool operator!=(T other) const
		{
			return !(*this == other);
		}

		template <class T> ML_NODISCARD constexpr bool operator<(T other) const
		{
			if constexpr (std::is_same_v<T, self_type>)
			{
				return m_words < other.m_words;
			}
			else
			{
				return (*this) < self_type{ other };
			}
		}

		template <class T> ML_NODISCARD constexpr bool operator>(T other) const
		{
			return !(*this < other);
		}

		template <class T> ML_NODISCARD constexpr bool operator<=(T other) const
		{
			return (*this < other) || (*this == other);
		}

		template <class T> ML_NODISCARD constexpr bool operator>=(T other) const
		{
			return (*this > other) || (*this == other);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_words;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t N> inline ML_SERIALIZE(std::ostream & out, bitset<N> const & value)
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