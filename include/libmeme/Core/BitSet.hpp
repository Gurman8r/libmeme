#ifndef _ML_BIT_SET_HPP_
#define _ML_BIT_SET_HPP_

#include <libmeme/Core/Array.hpp>

#define ML_bitread(v, i)		((v >> i) & 1)
#define ML_bitset(v, i)			(v |= (1 << i))
#define ML_bitclear(v, i)		(v &= ~(1 << i))
#define ML_bitwrite(v, i, b)	(b ? ML_bitset(v, i) : ML_bitclear(v, i))

namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t _Count = 64> struct bitset final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type = typename bitset<_Count>;

		using value_type = typename std::conditional_t<
			_Count <= sizeof(uint32_t) * 8,
			uint32_t,
			uint64_t
		>;

		static constexpr ptrdiff_t bitsperword{ sizeof(value_type) * 8 };
		
		static constexpr ptrdiff_t word_count{ (_Count - 1) / bitsperword };

		using storage_type = typename array<value_type, word_count + 1>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr bitset() noexcept : m_words{} {}

		template <class T, class = std::enable_if_t<
			std::is_integral_v<T>
		>> constexpr bitset(T const value) noexcept
			: m_words{ static_cast<value_type>(value) }
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

		constexpr bitset(array<bool, _Count> const & arr) noexcept
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
		>> constexpr self_type & operator=(T const value) noexcept
		{
			self_type temp{ value };
			swap(temp);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr operator bool() const noexcept
		{
			return m_words;
		}

		ML_NODISCARD constexpr operator storage_type const & () const & noexcept
		{
			return m_words;
		}

		ML_NODISCARD constexpr bool operator[](size_t const i) const noexcept
		{
			return read(i);
		}

		ML_NODISCARD constexpr array<bool, _Count> arr() const noexcept
		{
			array<bool, _Count> temp{};
			for (size_t i = 0; i < _Count; ++i)
				temp[i] = read(i);
			return temp;
		}

		template <size_t I> constexpr bool clear() noexcept
		{
			static_assert(I < _Count, "bitset subscript out of range");
			return clear(I);
		}

		constexpr bool clear(size_t const i) noexcept
		{
			bool const b{ read(i) };
			m_words[i / bitsperword] &= ~(value_type{ 1 } << i % bitsperword);
			return b;
		}

		template <size_t I> ML_NODISCARD constexpr bool read() const noexcept
		{
			static_assert(I < _Count, "bitset subscript out of range");
			return read(I);
		}

		ML_NODISCARD constexpr bool read(size_t const i) const noexcept
		{
			return m_words[i / bitsperword] & (value_type{ 1 } << i % bitsperword);
		}

		constexpr void reset() noexcept { m_words = { 0 }; }

		template <size_t I> constexpr bool set() noexcept
		{
			static_assert(I < _Count, "bitset subscript out of range");
			return set(I);
		}

		constexpr bool set(size_t const i) noexcept
		{
			bool const b{ !read(i) };
			m_words[i / bitsperword] |= (value_type{ 1 } << i % bitsperword);
			return b;
		}

		ML_NODISCARD constexpr size_t size() const noexcept
		{
			return _Count;
		}

		constexpr void swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				util::swap(m_words, other.m_words);
			}
		}

		ML_NODISCARD constexpr storage_type & words() noexcept
		{
			return m_words;
		}

		ML_NODISCARD constexpr storage_type const & words() const noexcept
		{
			return m_words;
		}

		template <size_t I> constexpr bool write(bool b) noexcept
		{
			static_assert(I < _Count, "bitset subscript out of range");
			return write(I, b);
		}

		constexpr bool write(size_t const i, bool b) noexcept
		{
			return b ? set(i) : clear(i);
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

	template <size_t N
	> inline std::ostream & operator<<(std::ostream & out, bitset<N> const & value)
	{
		for (size_t i = 0; i < N; ++i)
		{
			out << value.read(i);
		}
		return out;
	}

	template <size_t N
	> inline std::istream & operator>>(std::istream & in, bitset<N> & value)
	{
		for (size_t i = 0; i < N; ++i)
		{
			if (in.good())
			{
				bool b{}; in >> b;
				value.write(i, b);
			}
		}
		return in;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BIT_SET_HPP_