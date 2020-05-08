#ifndef _ML_UTILITY_HPP_
#define _ML_UTILITY_HPP_

#include <gcem/include/gcem.hpp>
#include <libmeme/Core/Flow.hpp>
#include <libmeme/Core/JSON.hpp>
#include <libmeme/Core/Meta.hpp>
#include <libmeme/Core/TypeOf.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// min / max / clamp
#define ML_min(a, b)			((a) < (b) ? (a) : (b))
#define ML_max(a, b)			((a) > (b) ? (a) : (b))
#define ML_clamp(v, lo, hi)		ML_min(ML_max(v, lo), hi)

// flag manipulation
#define ML_flag_read( v, f)		(v & f)
#define ML_flag_set(  v, f)		(v |= f)
#define ML_flag_clear(v, f)		(v &= ~f)
#define ML_flag_write(v, f, b)	(b ? ML_flag_set(v, f) : ML_flag_clear(v, f))

// bit manipulation
#define ML_bit_read( v, i)		ML_flag_read(v >> i, 1)
#define ML_bit_set(  v, i)		ML_flag_set(v, 1 << i)
#define ML_bit_clear(v, i)		ML_flag_clear(v, 1 << i)
#define ML_bit_write(v, i, b)	(b ? ML_bit_set(v, i) : ML_bit_clear(v, i))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Misc
namespace ml::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// is any of
	template <class T, class ... Ts
	> constexpr bool is_any_of_v{ std::disjunction_v<std::is_same<T, Ts>...> };

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

	template <class Lhs, class Rhs = Lhs
	> ML_NODISCARD constexpr auto compare(Lhs && lhs, Rhs && rhs) noexcept
	{
		return (ML_forward(lhs) != ML_forward(rhs))
			? ((ML_forward(lhs) < ML_forward(rhs)) ? -1 : 1)
			: 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class LI, class RI
	> ML_NODISCARD constexpr bool range_equal(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
	{
		return (lBegin != lEnd && rBegin != rEnd)
			? ((*lBegin == *rBegin) && _ML util::range_equal(lBegin + 1, lEnd, rBegin + 1, rEnd))
			: (lBegin == lEnd && rBegin == rEnd);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class LI, class RI
	> ML_NODISCARD constexpr bool range_less(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
	{
		return (lBegin != lEnd && rBegin != rEnd)
			? ((*lBegin < *rBegin) && _ML util::range_less(lBegin + 1, lEnd, rBegin + 1, rEnd))
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
			? _ML util::min(ML_forward(lhs), ML_forward(rest)...)
			: _ML util::min(ML_forward(rhs), ML_forward(rest)...);
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
			? _ML util::max(ML_forward(lhs), ML_forward(rest)...)
			: _ML util::max(ML_forward(rhs), ML_forward(rest)...);
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
		for (size_t i = 0; i < lhs.size(); ++i)
			temp += (lhs[i] * rhs[i]);
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
		return gcem::sqrt(sqr_magnitude<A, T, N...>(value));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class, size_t ...> class A, class T, size_t ... N
	> ML_NODISCARD constexpr auto normalize(A<T, N...> const & value)
	{
		return (value / magnitude<A, T, N...>(value));
	}

	template <template <class, size_t ...> class A, class T, size_t ... N
	> ML_NODISCARD constexpr auto transpose(A<T, N...> const & value)
	{
		A<T, N...> temp{};
		for (size_t i = 0; i < value.size(); ++i)
		{
			size_t const y{ i % value.width() }, x{ i / value.width() };
			
			temp[i] = value[y * value.width() + x];
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class, size_t, size_t> class M, class T
	> ML_NODISCARD constexpr auto inverse(M<T, 4, 4> const & v)
	{
		auto const det{ determinant<M, T>(v) };
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
		return (T{} < v[1]) ? (static_cast<T>(v[0]) / static_cast<T>(v[1])) : T{};
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
			return gcem::round(gcem::pow(2, gcem::ceil(gcem::log(v) / gcem::log(2))));
		}
		else if constexpr (std::is_integral_v<T>)
		{
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
namespace ml::literals
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// kibibyte
	ML_NODISCARD constexpr uint64_t operator"" _KiB(uint64_t n) noexcept
	{
		return util::power_of_2(util::ratio_cast(n, std::kilo{}));
	}

	ML_NODISCARD constexpr uint64_t operator"" _KiB(float80_t n) noexcept
	{
		return util::power_of_2(static_cast<uint64_t>(util::ratio_cast(n, std::kilo{})));
	}

	// mebibyte
	ML_NODISCARD constexpr uint64_t operator"" _MiB(uint64_t n) noexcept
	{
		return util::power_of_2(util::ratio_cast(n, std::mega{}));
	}

	ML_NODISCARD constexpr uint64_t operator"" _MiB(float80_t n) noexcept
	{
		return util::power_of_2(static_cast<uint64_t>(util::ratio_cast(n, std::mega{})));
	}

	// gibibyte
	ML_NODISCARD constexpr uint64_t operator"" _GiB(uint64_t n) noexcept
	{
		return util::power_of_2(util::ratio_cast(n, std::giga{}));
	}

	ML_NODISCARD constexpr uint64_t operator"" _GiB(float80_t n) noexcept
	{
		return util::power_of_2(static_cast<uint64_t>(util::ratio_cast(n, std::giga{})));
	}

	// tebibyte
	ML_NODISCARD constexpr uint64_t operator"" _TiB(uint64_t n) noexcept
	{
		return util::power_of_2(util::ratio_cast(n, std::tera{}));
	}

	ML_NODISCARD constexpr uint64_t operator"" _TiB(float80_t n) noexcept
	{
		return util::power_of_2(static_cast<uint64_t>(util::ratio_cast(n, std::tera{})));
	}

	// pebibyte
	ML_NODISCARD constexpr uint64_t operator"" _PiB(uint64_t n) noexcept
	{
		return util::power_of_2(util::ratio_cast(n, std::peta{}));
	}

	ML_NODISCARD constexpr uint64_t operator"" _PiB(float80_t n) noexcept
	{
		return util::power_of_2(static_cast<uint64_t>(util::ratio_cast(n, std::peta{})));
	}

	// exbibyte
	ML_NODISCARD constexpr uint64_t operator"" _EiB(uint64_t n) noexcept
	{
		return util::power_of_2(util::ratio_cast(n, std::exa{}));
	}

	ML_NODISCARD constexpr uint64_t operator"" _EiB(float80_t n) noexcept
	{
		return util::power_of_2(static_cast<uint64_t>(util::ratio_cast(n, std::exa{})));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_UTILITY_HPP_