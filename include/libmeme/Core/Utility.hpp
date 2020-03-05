#ifndef _ML_ALG_HPP_
#define _ML_ALG_HPP_

#include <libmeme/Core/Meta.hpp>
#include <libmeme/Core/ScopeGuard.hpp>
#include <gcem/include/gcem.hpp>

#define _ML_UTIL _ML util::

#define ML_BIT_READ(v, i)		((v >> i) & 1)
#define ML_BIT_SET(v, i)		(v |= (1 << i))
#define ML_BIT_CLEAR(v, i)		(v &= ~(1 << i))
#define ML_BIT_WRITE(v, i, b)	(b ? ML_BIT_SET(v, i) : ML_BIT_CLEAR(v, i))

#define ML_ASPECT(w, h)			((w != 0 && h != 0) ? ((float_t)w / (float_t)h) : 0.f)
#define ML_ASPECT2(v)			ML_ASPECT(v[0], v[1])

#define ML_MIN(lhs, rhs)		(lhs <= rhs ? lhs : rhs)
#define ML_MAX(lhs, rhs)		(lhs >= rhs ? lhs : rhs)
#define ML_CLAMP(v, lo, hi)		ML_MIN(ML_MAX(v, lo), hi)

// General
namespace ml::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch
	> ML_NODISCARD constexpr size_t strlen(Ch const * value)
	{
		return ((*value) ? (1 + _ML_UTIL strlen(value + 1)) : 0);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> constexpr void swap(T & lhs, T & rhs) noexcept
	{
		T temp = std::move(lhs);
		lhs = std::move(rhs);
		rhs = std::move(temp);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class LI, class RI
	> ML_NODISCARD constexpr bool equal_to(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
	{
		return (lBegin != lEnd && rBegin != rEnd)
			? (((*lBegin) == (*rBegin))
				&& _ML_UTIL equal_to((lBegin + 1), lEnd, (rBegin + 1), rEnd))
			: ((lBegin == lEnd) && (rBegin == rEnd));
	}

	template <class LI, class RI
	> ML_NODISCARD constexpr bool less(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
	{
		return (lBegin != lEnd && rBegin != rEnd)
			? (((*lBegin) < (*rBegin))
				&& _ML_UTIL less((lBegin + 1), lEnd, (rBegin + 1), rEnd))
			: ((lBegin == lEnd) && (rBegin == rEnd));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Lhs, class Rhs, class ... Rest
	> constexpr decltype(auto) min(Lhs && lhs, Rhs && rhs, Rest && ... rest)
	{
		return lhs < rhs
			? _ML_UTIL min(ML_FWD(lhs), ML_FWD(rest)...)
			: _ML_UTIL min(ML_FWD(rhs), ML_FWD(rest)...);
	}

	template <class Only
	> constexpr decltype(auto) min(Only && only)
	{
		return ML_FWD(only);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Lhs, class Rhs, class ... Rest
	> constexpr decltype(auto) max(Lhs && lhs, Rhs && rhs, Rest && ... rest)
	{
		return lhs > rhs
			? _ML_UTIL max(ML_FWD(lhs), ML_FWD(rest)...)
			: _ML_UTIL max(ML_FWD(rhs), ML_FWD(rest)...);
	}

	template <class Only
	> constexpr decltype(auto) max(Only && only)
	{
		return ML_FWD(only);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Lhs, class Rhs
	> constexpr decltype(auto) clamp(T && v, Lhs && lhs, Rhs && rhs)
	{
		return _ML_UTIL min(_ML_UTIL max(ML_FWD(v), ML_FWD(lhs)), ML_FWD(rhs));
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
				v |= v >> 1; v |= v >> 2; v |= v >> 4;	// 8
			if constexpr (sizeof(T) >= 2) v |= v >> 8;	// 16
			if constexpr (sizeof(T) >= 4) v |= v >> 16; // 32
			if constexpr (sizeof(T) >= 8) v |= v >> 32; // 64
			v++;
			return v;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Hash
namespace ml::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static constexpr auto fnv1a_basis{ static_cast<hash_t>(14695981039346656037ULL) };

	static constexpr auto fnv1a_prime{ static_cast<hash_t>(1099511628211ULL) };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> ML_NODISCARD constexpr hash_t hash(T const * arr, hash_t size, hash_t seed) noexcept
	{
		return (size > 0)
			? _ML_UTIL hash(arr + 1, size - 1, (seed ^ static_cast<hash_t>(*arr)) * fnv1a_prime)
			: seed;
	}

	template <class T
	> ML_NODISCARD constexpr hash_t hash(T const * arr, hash_t size) noexcept
	{
		return _ML_UTIL hash(arr, size, fnv1a_basis);
	}

	template <class T, hash_t N
	> ML_NODISCARD constexpr hash_t hash(const T(&value)[N]) noexcept
	{
		return _ML_UTIL hash(value, N - 1);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Arr
	> ML_NODISCARD constexpr hash_t hash(Arr const & value) noexcept
	{
		return _ML_UTIL hash(value.data(), static_cast<hash_t>(value.size()));
	}

	template <template <class, hash_t...> class Arr, class T, hash_t ... N
	> ML_NODISCARD constexpr hash_t hash(Arr<T, N...> const & value) noexcept
	{
		return _ML_UTIL hash(value.data(), static_cast<hash_t>(value.size()));
	}

	template <template <class...> class Arr, class ... Ts
	> ML_NODISCARD constexpr hash_t hash(Arr<Ts...> const & value) noexcept
	{
		return _ML_UTIL hash(value.data(), static_cast<hash_t>(value.size()));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Vector Math
namespace ml::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class, size_t ...> class A, class T, size_t ... N
	> ML_NODISCARD constexpr auto dot(A<T, N...> const & lhs, A<T, N...> const & rhs)
	{
		T temp{ 0 };
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
		T temp{ (T)0 };
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

	template <template <class, size_t, size_t> class M, class T
	> ML_NODISCARD constexpr auto scale_to_fit(M<T, 2, 1> const & l, M<T, 2, 1> const & r)
	{
		const M<T, 2, 1>
			h{ { (r[0] / l[0]), (r[0] / l[0]) } },
			v{ { (r[1] / l[1]), (r[1] / l[1]) } };
		return l * ((h < v) ? h : v);
	}

	template <class Src, class Dst
	> ML_NODISCARD constexpr Src maintain(Src const & src, Dst const & dst)
	{
		Dst const v{ dst[0] / src[0], dst[1] / src[1] };
		auto const r{ v[0] < v[1] ? v[0] : v[1] };
		return Src{ src[0] * r, src[1] * r };
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
		A<T, N...> temp{ 0 };
		for (size_t i = 0; i < value.size(); ++i)
		{
			size_t const y{ i % value.width() };
			size_t const x{ i / value.width() };
			temp[i] = value[y * value.width() + x];
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class, size_t, size_t> class M, class T
	> ML_NODISCARD constexpr auto inverse(M<T, 4, 4> const & v)
	{
		T const det{ determinant<M, T>(v) };
		return ((det != (T)0)
			? M<T, 4, 4> {
			+(v[15] * v[5] - v[7] * v[13]) / det,
				-(v[15] * v[4] - v[7] * v[12]) / det,
				+(v[13] * v[4] - v[5] * v[12]) / det,
				-(v[15] * v[1] - v[3] * v[13]) / det,
				+(v[15] * v[0] - v[3] * v[12]) / det,
				-(v[13] * v[0] - v[1] * v[12]) / det,
				+(v[7] * v[1] - v[3] * v[5]) / det,
				-(v[7] * v[0] - v[3] * v[4]) / det,
				+(v[5] * v[0] - v[1] * v[4]) / det
		}
		: M<T, 4, 4>::identity()
			);
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
		return M<T, 3, 1> {
			a[1] * b[2] - b[1] * a[2],
				a[2] * b[0] - b[2] * a[0],
				a[0] * b[1] - b[0] * a[1]
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Literals
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
		return static_cast<uint64_t>(util::power_of_2(util::ratio_cast(n, std::kilo{})));
	}

	// mebibyte
	ML_NODISCARD constexpr uint64_t operator"" _MiB(uint64_t n) noexcept
	{
		return util::power_of_2(util::ratio_cast(n, std::mega{}));
	}

	ML_NODISCARD constexpr uint64_t operator"" _MiB(float80_t n) noexcept
	{
		return static_cast<uint64_t>(util::power_of_2(util::ratio_cast(n, std::mega{})));
	}

	// gibibyte
	ML_NODISCARD constexpr uint64_t operator"" _GiB(uint64_t n) noexcept
	{
		return util::power_of_2(util::ratio_cast(n, std::giga{}));
	}

	ML_NODISCARD constexpr uint64_t operator"" _GiB(float80_t n) noexcept
	{
		return static_cast<uint64_t>(util::power_of_2(util::ratio_cast(n, std::giga{})));
	}

	// tebibyte
	ML_NODISCARD constexpr uint64_t operator"" _TiB(uint64_t n) noexcept
	{
		return util::power_of_2(util::ratio_cast(n, std::tera{}));
	}

	ML_NODISCARD constexpr uint64_t operator"" _TiB(float80_t n) noexcept
	{
		return static_cast<uint64_t>(util::power_of_2(util::ratio_cast(n, std::tera{})));
	}

	// pebibyte
	ML_NODISCARD constexpr uint64_t operator"" _PiB(uint64_t n) noexcept
	{
		return util::power_of_2(util::ratio_cast(n, std::peta{}));
	}

	ML_NODISCARD constexpr uint64_t operator"" _PiB(float80_t n) noexcept
	{
		return static_cast<uint64_t>(util::power_of_2(util::ratio_cast(n, std::peta{})));
	}

	// exbibyte
	ML_NODISCARD constexpr uint64_t operator"" _EiB(uint64_t n) noexcept
	{
		return util::power_of_2(util::ratio_cast(n, std::exa{}));
	}

	ML_NODISCARD constexpr uint64_t operator"" _EiB(float80_t n) noexcept
	{
		return static_cast<uint64_t>(util::power_of_2(util::ratio_cast(n, std::exa{})));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ALG_HPP_