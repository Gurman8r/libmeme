#ifndef _ML_UTILITY_HPP_
#define _ML_UTILITY_HPP_

#include <gcem/include/gcem.hpp>
#include <libmeme/Core/Flow.hpp>
#include <libmeme/Core/JSON.hpp>
#include <libmeme/Core/Meta.hpp>
#include <libmeme/Core/TypeInfo.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// macro min
#define ML_min(a, b)				((a) < (b) ? (a) : (b))

// macro max
#define ML_max(a, b)				((a) > (b) ? (a) : (b))

// macro clamp
#define ML_clamp(v, lo, hi)			ML_min(ML_max(v, lo), hi)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// read flag at index
#define ML_flag_read(val, i)		(bool)((val) & (i))

// set flag at index
#define ML_flag_set(val, i)			((val) |= (i))

// clear flag at index
#define ML_flag_clear(val, i)		((val) &= ~(i))

// conditional set or clear flag
#define ML_flag_write(val, i, cond)	((cond) ? ML_flag_set(val, i) : ML_flag_clear(val, i))

// map between flag bits
#define ML_flag_map(dst, d, src, s)	ML_flag_write(dst, d, ML_flag_read(src, s))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// read bit at index
#define ML_bit_read(val, i)			ML_flag_read(val >> i, 1)

// set bit at index
#define ML_bit_set(val, i)			ML_flag_set(val, 1 << i)

// clear bit at index
#define ML_bit_clear(val, i)		ML_flag_clear(val, 1 << i)

// conditional set or clear bit
#define ML_bit_write(val, i, cond)	ML_flag_write(val, 1 << i, cond)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Misc
namespace ml::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// requires T is any of
	template <class T, class ... Ts
	> constexpr bool is_any_of_v
	{
		std::disjunction_v<std::is_same<T, Ts>...>
	};

	// requires T is integral or float
	template <class T
	> constexpr bool is_integral_or_floating_point_v
	{
		std::is_enum_v<T> || std::is_integral_v<T> || std::is_floating_point_v<T>
	};

	// requires To is trivially default constructible and is copy/move constructible
	template <class To, class From
	> static constexpr bool is_trivial_conversion_v
	{
		(sizeof(To) == sizeof(From))		&&
		std::is_trivially_copyable_v<From>	&&
		std::is_trivial_v<To>				&&
		(std::is_copy_constructible_v<To> || std::is_move_constructible_v<To>)
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// bit cast
	template <class To, class From
	> To bit_cast(From const & value) noexcept
	{
		static_assert(is_trivial_conversion_v<To, From>, "invalid bit_cast");
		To temp{};
		std::memcpy(&temp, &value, sizeof(To));
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// constexpr swap
	template <class T
	> constexpr void swap(T & lhs, T & rhs) noexcept
	{
		T temp{ std::move(lhs) };
		lhs = std::move(rhs);
		rhs = std::move(temp);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Comparison
namespace ml::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Lhs, class Rhs
	> ML_NODISCARD constexpr auto compare(Lhs && lhs, Rhs && rhs) noexcept
	{
		return ML_compare(ML_forward(lhs), ML_forward(rhs));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class LI, class RI
	> ML_NODISCARD constexpr bool range_equal(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
	{
		return (lBegin != lEnd && rBegin != rEnd)
			? ((*lBegin == *rBegin) && _ML_UTIL range_equal(lBegin + 1, lEnd, rBegin + 1, rEnd))
			: (lBegin == lEnd && rBegin == rEnd);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class LI, class RI
	> ML_NODISCARD constexpr bool range_less(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
	{
		return (lBegin != lEnd && rBegin != rEnd)
			? ((*lBegin < *rBegin) && _ML_UTIL range_less(lBegin + 1, lEnd, rBegin + 1, rEnd))
			: (lBegin == lEnd && rBegin == rEnd);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Min / Max / Clamp
namespace ml::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Lhs, class Rhs, class ... Rest
	> constexpr decltype(auto) min(Lhs && lhs, Rhs && rhs, Rest && ... rest)
	{
		return lhs < rhs
			? _ML_UTIL min(ML_forward(lhs), ML_forward(rest)...)
			: _ML_UTIL min(ML_forward(rhs), ML_forward(rest)...);
	}

	template <class Only
	> constexpr decltype(auto) min(Only && only)
	{
		return ML_forward(only);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Lhs, class Rhs, class ... Rest
	> constexpr decltype(auto) max(Lhs && lhs, Rhs && rhs, Rest && ... rest)
	{
		return lhs > rhs
			? _ML_UTIL max(ML_forward(lhs), ML_forward(rest)...)
			: _ML_UTIL max(ML_forward(rhs), ML_forward(rest)...);
	}

	template <class Only
	> constexpr decltype(auto) max(Only && only)
	{
		return ML_forward(only);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Lo, class Hi
	> constexpr decltype(auto) clamp(T && value, Lo && lo, Hi && hi)
	{
		return ML_clamp(ML_forward(value), ML_forward(lo), ML_forward(hi));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Maths
namespace ml::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class, size_t ...> class A, class T, size_t ... N
	> ML_NODISCARD constexpr auto dot(A<T, N...> const & lhs, A<T, N...> const & rhs)
	{
		T temp{};
		for (size_t d = 0; d < lhs.size(); ++d)
			temp += (lhs[d] * rhs[d]);
		return temp;
	}

	template <template <class, size_t, size_t> class M, class T
	> ML_NODISCARD constexpr auto determinant(M<T, 4, 4> const & v)
	{
		return
			v[0] * (v[15] * v[5] - v[7] * v[13]) -
			v[1] * (v[15] * v[4] - v[7] * v[12]) +
			v[3] * (v[13] * v[4] - v[5] * v[12]);
	}

	template <template <class, size_t ...> class A, class T, size_t ... N
	> ML_NODISCARD constexpr auto sqr_magnitude(A<T, N...> const & value)
	{
		T temp{};
		for (auto const & elem : value)
			temp += (elem * elem);
		return temp;
	}

	template <template <class, size_t ...> class A, class T, size_t ... N
	> ML_NODISCARD constexpr auto magnitude(A<T, N...> const & value)
	{
		return gcem::sqrt(_ML_UTIL sqr_magnitude<A, T, N...>(value));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class, size_t ...> class A, class T, size_t ... N
	> ML_NODISCARD constexpr auto normalize(A<T, N...> const & value)
	{
		return (value / _ML_UTIL magnitude<A, T, N...>(value));
	}

	template <template <class, size_t, size_t> class M, class T, size_t W, size_t H
	> ML_NODISCARD constexpr auto transpose(M<T, W, H> const & value)
	{
		M<T, W, H> temp{};
		for (size_t d = 0; d < value.size(); ++d)
		{
			size_t const y{ d % value.width() }, x{ d / value.width() };
			
			temp[d] = value[y * value.width() + x];
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class, size_t, size_t> class M, class T
	> ML_NODISCARD constexpr auto inverse(M<T, 4, 4> const & v)
	{
		auto const det{ _ML_UTIL determinant<M, T>(v) };
		return (det == T{})
			? M<T, 4, 4>::identity()
			: M<T, 4, 4>
			{
				+(v[15] * v[5] - v[7] * v[13]) / det,
				-(v[15] * v[4] - v[7] * v[12]) / det,
				+(v[13] * v[4] - v[5] * v[12]) / det,
				-(v[15] * v[1] - v[3] * v[13]) / det,
				+(v[15] * v[0] - v[3] * v[12]) / det,
				-(v[13] * v[0] - v[1] * v[12]) / det,
				+(v[ 7] * v[1] - v[3] * v[ 5]) / det,
				-(v[ 7] * v[0] - v[3] * v[ 4]) / det,
				+(v[ 5] * v[0] - v[1] * v[ 4]) / det
			};
	}

	template <template <class, size_t, size_t> class M, class T
	> ML_NODISCARD constexpr auto rebase(const M<T, 3, 3> & v, M<T, 4, 4> const & m)
	{
		return M<T, 3, 3>
		{
			m[0] * v[0] + m[4] * v[3] + m[8] * v[6],
			m[1] * v[0] + m[5] * v[3] + m[9] * v[6],
			m[2] * v[0] + m[6] * v[3] + m[10] * v[6],
			m[0] * v[1] + m[4] * v[4] + m[8] * v[7],
			m[1] * v[1] + m[5] * v[4] + m[9] * v[7],
			m[2] * v[1] + m[6] * v[4] + m[10] * v[7],
			m[0] * v[2] + m[4] * v[5] + m[8] * v[8],
			m[1] * v[2] + m[5] * v[5] + m[9] * v[8],
			m[2] * v[2] + m[6] * v[5] + m[10] * v[8]
		};
	}

	template <template <class, size_t, size_t> class M, class T
	> ML_NODISCARD constexpr auto rebase(const M<T, 3, 1> & v, M<T, 4, 4> const & m)
	{
		return M<T, 3, 1>
		{
			m[0] * v[0] * m[4] * v[1] * m[8] * v[2] * m[12],
			m[1] * v[0] * m[5] * v[1] * m[9] * v[2] * m[13],
			m[2] * v[0] * m[6] * v[1] * m[10] * v[2] * m[14]
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class, size_t, size_t> class M, class T
	> ML_NODISCARD constexpr auto cross(M<T, 2, 1> const & a, M<T, 2, 1> const & b)
	{
		return a[0] * b[1] - b[0] * a[1];
	}

	template <template <class, size_t, size_t> class M, class T
	> ML_NODISCARD constexpr auto cross(const M<T, 3, 1> & a, const M<T, 3, 1> & b)
	{
		return M<T, 3, 1>
		{
			a[1] * b[2] - b[1] * a[2],
			a[2] * b[0] - b[2] * a[0],
			a[0] * b[1] - b[0] * a[1]
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class V2, class T = float_t
	> ML_NODISCARD constexpr auto aspect(V2 const & v) noexcept
	{
		return (T{ 0 } < v[1]) ? (static_cast<T>(v[0]) / static_cast<T>(v[1])) : T{ 0 };
	}

	template <class V2
	> ML_NODISCARD constexpr auto scale_to_fit(V2 const & lhs, V2 const & rhs) noexcept
	{
		auto const
			h{ V2{ (rhs[0] / lhs[0]), (rhs[0] / lhs[0]) } },
			v{ V2{ (rhs[1] / lhs[1]), (rhs[1] / lhs[1]) } };
		return lhs * ((h < v) ? h : v);
	}

	template <class Src, class Dst
	> ML_NODISCARD constexpr auto maintain(Src const & source, Dst const & target) noexcept
	{
		Dst const temp{ target[0] / source[0], target[1] / source[1] };
		auto const ratio{ (temp[0] < temp[1]) ? temp[0] : temp[1] };
		return Src{ source[0] * ratio, source[1] * ratio };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, int64_t Num, int64_t Den
	> ML_NODISCARD constexpr auto ratio_cast(T v, std::ratio<Num, Den> const & r)
	{
		auto const
			one{ static_cast<T>(1) },
			num{ static_cast<T>(r.num) },
			den{ static_cast<T>(r.den) };
		return ((num == one) && (den == one))
			? v
			: (((num != one) && (den == one))
				? v * num
				: (((num == one) && (den != one))
					? v / den
					: v * num / den));
	}

	template <class T
	> ML_NODISCARD constexpr auto power_of_2(T v)
	{
		// https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
		if constexpr (std::is_floating_point_v<T>)
		{
			return gcem::round(gcem::pow(T{ 2 }, gcem::ceil(gcem::log(v) / gcem::log(T{ 2 }))));
		}
		else
		{
			static_assert(std::is_integral_v<T>);
			v--;
			if constexpr (sizeof(T) >= 1)
				v |= v >> 1; v |= v >> 2; v |= v >> 4;	// 8-bit
			if constexpr (sizeof(T) >= 2) v |= v >> 8;	// 16-bit
			if constexpr (sizeof(T) >= 4) v |= v >> 16; // 32-bit
			if constexpr (sizeof(T) >= 8) v |= v >> 32; // 64-bit
			v++;
			return v;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Size Literals
namespace ml::size_literals
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// kibibyte
	ML_NODISCARD constexpr uint64_t operator"" _KiB(uint64_t n) noexcept
	{
		return _ML_UTIL power_of_2(_ML_UTIL ratio_cast(n, std::kilo{}));
	}

	ML_NODISCARD constexpr uint64_t operator"" _KiB(float80_t n) noexcept
	{
		return static_cast<uint64_t>(_ML_UTIL power_of_2(_ML_UTIL ratio_cast(n, std::kilo{})));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// mebibyte
	ML_NODISCARD constexpr uint64_t operator"" _MiB(uint64_t n) noexcept
	{
		return _ML_UTIL power_of_2(_ML_UTIL ratio_cast(n, std::mega{}));
	}

	ML_NODISCARD constexpr uint64_t operator"" _MiB(float80_t n) noexcept
	{
		return static_cast<uint64_t>(_ML_UTIL power_of_2(_ML_UTIL ratio_cast(n, std::mega{})));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// gibibyte
	ML_NODISCARD constexpr uint64_t operator"" _GiB(uint64_t n) noexcept
	{
		return _ML_UTIL power_of_2(_ML_UTIL ratio_cast(n, std::giga{}));
	}

	ML_NODISCARD constexpr uint64_t operator"" _GiB(float80_t n) noexcept
	{
		return static_cast<uint64_t>(_ML_UTIL power_of_2(_ML_UTIL ratio_cast(n, std::giga{})));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// tebibyte
	ML_NODISCARD constexpr uint64_t operator"" _TiB(uint64_t n) noexcept
	{
		return _ML_UTIL power_of_2(_ML_UTIL ratio_cast(n, std::tera{}));
	}

	ML_NODISCARD constexpr uint64_t operator"" _TiB(float80_t n) noexcept
	{
		return static_cast<uint64_t>(_ML_UTIL power_of_2(_ML_UTIL ratio_cast(n, std::tera{})));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// pebibyte
	ML_NODISCARD constexpr uint64_t operator"" _PiB(uint64_t n) noexcept
	{
		return _ML_UTIL power_of_2(_ML_UTIL ratio_cast(n, std::peta{}));
	}

	ML_NODISCARD constexpr uint64_t operator"" _PiB(float80_t n) noexcept
	{
		return static_cast<uint64_t>(_ML_UTIL power_of_2(_ML_UTIL ratio_cast(n, std::peta{})));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// exbibyte
	ML_NODISCARD constexpr uint64_t operator"" _EiB(uint64_t n) noexcept
	{
		return _ML_UTIL power_of_2(_ML_UTIL ratio_cast(n, std::exa{}));
	}

	ML_NODISCARD constexpr uint64_t operator"" _EiB(float80_t n) noexcept
	{
		return static_cast<uint64_t>(_ML_UTIL power_of_2(_ML_UTIL ratio_cast(n, std::exa{})));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UTILITY_HPP_