#ifndef _ML_ALG_HPP_
#define _ML_ALG_HPP_

#include <libmeme/Common.hpp>
#include <gcem/include/gcem.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define _ML_ALG ::ml::alg::

/* * * * * * * * * * * * * * * * * * * * */

namespace ml::alg
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> static constexpr void swap(T & lhs, T & rhs) noexcept
	{
		T temp{ std::move(lhs) };
		lhs = std::move(rhs);
		rhs = std::move(temp);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch
	> ML_NODISCARD static constexpr size_t strlen(Ch const * value)
	{
		return ((*value) ? (1 + _ML_ALG strlen(value + 1)) : 0);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class To, class From, class = std::enable_if_t<
		(sizeof(To) == sizeof(From)) &&
		std::is_trivially_copyable_v<From> &&
		std::is_trivial_v<To>, To>
	> ML_NODISCARD static inline To bit_cast(From const & src) noexcept
	{
		To dst;
		std::memcpy(&dst, &src, sizeof(To));
		return dst;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> ML_NODISCARD static constexpr int32_t sign(T const & value)
	{
		return ((value == (T)0)
			? (T)0
			: ((value < (T)0)
				? (T)-1
				: (T)1
		));
	}

	template <class T
	> ML_NODISCARD static constexpr T abs(T const & value)
	{
		return ((_ML_ALG sign(value) < (T)0)
			? (value * (T)-1)
			: value
		);
	}

	template <class T
	> ML_NODISCARD static constexpr T fact(T const & value)
	{
		return ((value > (T)1) ? value * _ML_ALG fact(value - (T)1) : (T)1);
	}

	template <class Base, class Exp
	> ML_NODISCARD static constexpr Base pow(Base const & base, Exp const & exp)
	{
		using B = cast_t<Base>;
		using E = cast_t<Exp>;
		return ((exp < E::zero)
			? ((base == B::zero)
				? limits_t<Base>::nan
				: B::one / (base * _ML_ALG pow(base, (-exp) - E::one)))
			: ((exp == E::zero)
				? B::one
				: ((exp == E::one)
					? base
					: base * _ML_ALG pow(base, exp - E::one)
					)));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> ML_NODISCARD static constexpr decltype(auto) min(T const & lhs, T const & rhs)
	{
		return (lhs <= rhs) ? lhs : rhs;
	}

	template <class T
	> ML_NODISCARD static constexpr decltype(auto) max(T const & lhs, T const & rhs)
	{
		return (lhs >= rhs) ? lhs : rhs;
	}

	template <class T
	> ML_NODISCARD static constexpr decltype(auto) clamp(T const & value, T const & lower, T const & upper)
	{
		return _ML_ALG min(_ML_ALG max(value, lower), upper);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace impl
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... T> struct sqrt;

		template <> struct sqrt<> final { sqrt() = delete; };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <> struct sqrt<uintmax_t>
		{
			using type = typename uintmax_t;

			ML_NODISCARD constexpr type operator()(type value) const
			{
				return sqrt<type>{}(cast_t<type>::one, cast_t<type>::three, value);
			}

			ML_NODISCARD constexpr type operator()(type value, type curr, type prev) const
			{
				return (value <= prev)
					? sqrt<type>{}(value + curr, curr + cast_t<type>::two, prev)
					: (curr >> cast_t<type>::one) - cast_t<type>::one;
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <> struct sqrt<float64_t>
		{
			using type = typename float64_t;

			ML_NODISCARD constexpr type operator()(type value, type curr, type prev) const
			{
				return (curr == prev)
					? curr
					: sqrt<type>{}(value, cast_t<type>::half * (curr + value / curr), curr);
			}

			ML_NODISCARD constexpr type operator()(type value) const
			{
				return (value >= cast_t<type>::zero && value < limits_t<type>::infinity)
					? sqrt<type>{}(value, value, cast_t<type>::zero)
					: limits_t<type>::nan;
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class To, class From> struct sqrt<To, From> : public sqrt<From>
		{
			template <class U> ML_NODISCARD constexpr To operator()(U const & value) const
			{
				return static_cast<To>(sqrt<From>{}(static_cast<From>(value)));
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <> struct sqrt<intmax_t>
		{
			ML_NODISCARD constexpr intmax_t operator()(intmax_t value) const
			{
				return sqrt<intmax_t, uintmax_t>{}(value);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <> struct sqrt<float32_t>
		{
			ML_NODISCARD constexpr float32_t operator()(float32_t value) const
			{
				return sqrt<float32_t, float64_t>{}(value);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct sqrt final : public ::ml::alg::impl::sqrt<T>
	{
		ML_NODISCARD constexpr T operator()(T value) const
		{
			return ::ml::alg::impl::sqrt<T>{}(value);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Coeff
	> ML_NODISCARD static constexpr T lerp(T const & a, T const & b, Coeff const & c)
	{
		return (a * c + b * ((Coeff)1 - c));
	}

	template <class T
	> ML_NODISCARD static constexpr T map(T value, T a0, T a1, T b0, T b1)
	{
		return (b0 + (value - a0) * (b1 - b0) / (a1 - a0));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t N
	> ML_NODISCARD static constexpr T const * begin(const T(&value)[N])
	{
		return (&value[0]);
	}

	template <class T, size_t N
	> ML_NODISCARD static constexpr T const * end(const T(&value)[N])
	{
		return (&value[N]);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class LI, class RI
	> ML_NODISCARD static constexpr bool equals(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
	{
		return ((lBegin != lEnd && rBegin != rEnd)
			? (((*lBegin) == (*rBegin))
				&& _ML_ALG equals((lBegin + 1), lEnd, (rBegin + 1), rEnd))
			: ((lBegin == lEnd) && (rBegin == rEnd))
		);
	}

	template <class LI, class RI
	> ML_NODISCARD static constexpr bool less(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
	{
		return ((lBegin != lEnd && rBegin != rEnd)
			? (((*lBegin) < (*rBegin))
				&& _ML_ALG less((lBegin + 1), lEnd, (rBegin + 1), rEnd))
			: ((lBegin == lEnd) && (rBegin == rEnd))
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class It, class Cmp
	> ML_NODISCARD static constexpr It find_if(It first, It last, Cmp compare)
	{
		while (first != last)
		{
			if (compare(*first))
			{
				return first;
			}
			++first;
		}
		return last;
	}

	template <class T, size_t N
	> ML_NODISCARD static constexpr size_t array_size(const T(&value)[N])
	{
		return static_cast<size_t>(ML_ARRAYSIZE(value));
	}

	template <class T, size_t N
	> ML_NODISCARD static constexpr int32_t index_of(T const & value, const T(&arr)[N])
	{
		for (int32_t i = 0; (&arr[i]) != (&arr[N]); ++i)
		{
			if (arr[i] == value) { return i; }
		}
		return -1;
	}

	template <class T, size_t N
	> ML_NODISCARD static constexpr T const & value_at(int32_t index, const T(&arr)[N])
	{
		return arr[index];
	}

	template <class T, size_t N
	> ML_NODISCARD static constexpr bool value_at(int32_t index, T & value, const T(&arr)[N])
	{
		if (index < N)
		{
			value = _ML_ALG value_at(index, arr);
			return true;
		}
		return false;
	}

	template <class T, size_t N
	> ML_NODISCARD static constexpr auto at_index(int32_t index, const T(&arr)[N])
	{
		return ((index >= 0 && index < N) ? arr[index] : nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class, size_t ...> class A, class T, size_t ... N
	> ML_NODISCARD static constexpr T dot(const A<T, N...> & lhs, const A<T, N...> & rhs)
	{
		T temp { 0 };
		for (size_t i = 0; i < lhs.size(); ++i)
		{
			temp += (lhs[i] * rhs[i]);
		}
		return temp;
	}

	template <template <class, size_t, size_t> class M, class T
	> ML_NODISCARD static constexpr T determinant(const M<T, 4, 4> & v)
	{
		return
			v[0] * (v[15] * v[5] - v[7] * v[13]) -
			v[1] * (v[15] * v[4] - v[7] * v[12]) +
			v[3] * (v[13] * v[4] - v[5] * v[12]);
	}

	template <template <class, size_t ...> class A, class T, size_t ... N
	> ML_NODISCARD static constexpr T sqr_magnitude(const A<T, N...> & value)
	{
		T temp { (T)0 };
		for (auto const & elem : value)
		{
			temp += (elem * elem);
		}
		return temp;
	}

	template <template <class, size_t ...> class A, class T, size_t ... N
	> ML_NODISCARD static constexpr T magnitude(const A<T, N...> & value)
	{
		return sqrt<T>{}(_ML_ALG sqr_magnitude<A, T, N...>(value));
	}

	template <template <class, size_t, size_t> class M, class T
	> ML_NODISCARD static constexpr M<T, 2, 1> scale_to_fit(const M<T, 2, 1> & l, const M<T, 2, 1> & r)
	{
		const M<T, 2, 1>
			h { { (r[0] / l[0]), (r[0] / l[0]) } },
			v { { (r[1] / l[1]), (r[1] / l[1]) } };
		return l * ((h < v) ? h : v);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class, size_t ...> class A, class T, size_t ... N
	> ML_NODISCARD static constexpr A<T, N...> normalize(const A<T, N...> & value)
	{
		return (value / _ML_ALG magnitude<A, T, N...>(value));
	}

	template <template <class, size_t ...> class A, class T, size_t ... N
	> ML_NODISCARD static constexpr A<T, N...> transpose(const A<T, N...> & value)
	{
		A<T, N...> temp { 0 };
		for (size_t i = 0; i < value.size(); ++i)
		{
			size_t const y { i % value.width() };
			size_t const x { i / value.width() };
			temp[i] = value[y * value.width() + x];
		}
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class, size_t, size_t> class M, class T
	> ML_NODISCARD static constexpr M<T, 4, 4> inverse(const M<T, 4, 4> & v)
	{
		const T det { _ML_ALG determinant<M, T>(v) };
		return ((det != (T)0)
			? M<T, 4, 4> {	
				+(v[15] * v[5] - v[7] * v[13]) / det,
				-(v[15] * v[4] - v[7] * v[12]) / det,
				+(v[13] * v[4] - v[5] * v[12]) / det,
				-(v[15] * v[1] - v[3] * v[13]) / det,
				+(v[15] * v[0] - v[3] * v[12]) / det,
				-(v[13] * v[0] - v[1] * v[12]) / det,
				+(v[ 7] * v[1] - v[3] * v[ 5]) / det,
				-(v[ 7] * v[0] - v[3] * v[ 4]) / det,
				+(v[ 5] * v[0] - v[1] * v[ 4]) / det
			}
			: M<T, 4, 4>::identity()
		);
	}

	template <template <class, size_t, size_t> class M, class T
	> ML_NODISCARD static constexpr M<T, 3, 3> rebase(const M<T, 3, 3> & v, const M<T, 4, 4> & m)
	{
		return M<T, 3, 3>
		{
			m[0] * v[0] + m[4] * v[3] + m[ 8] * v[6],
			m[1] * v[0] + m[5] * v[3] + m[ 9] * v[6],
			m[2] * v[0] + m[6] * v[3] + m[10] * v[6],
			m[0] * v[1] + m[4] * v[4] + m[ 8] * v[7],
			m[1] * v[1] + m[5] * v[4] + m[ 9] * v[7],
			m[2] * v[1] + m[6] * v[4] + m[10] * v[7],
			m[0] * v[2] + m[4] * v[5] + m[ 8] * v[8],
			m[1] * v[2] + m[5] * v[5] + m[ 9] * v[8],
			m[2] * v[2] + m[6] * v[5] + m[10] * v[8]
		};
	}

	template <template <class, size_t, size_t> class M, class T
	> ML_NODISCARD static constexpr M<T, 3, 1> rebase(const M<T, 3, 1> & v, const M<T, 4, 4> & m)
	{
		return M<T, 3, 1>
		{
			m[0] * v[0] * m[4] * v[1] * m[ 8] * v[2] * m[12],
			m[1] * v[0] * m[5] * v[1] * m[ 9] * v[2] * m[13],
			m[2] * v[0] * m[6] * v[1] * m[10] * v[2] * m[14]
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class, size_t, size_t> class M, class T
	> ML_NODISCARD static constexpr T cross(const M<T, 2, 1> & a, const M<T, 2, 1> & b)
	{
		return a[0] * b[1] - b[0] * a[1];
	}

	template <template <class, size_t, size_t> class M, class T
	> ML_NODISCARD static constexpr M<T, 3, 1> cross(const M<T, 3, 1> & a, const M<T, 3, 1> & b)
	{
		return M<T, 3, 1> {
			a[1] * b[2] - b[1] * a[2],
			a[2] * b[0] - b[2] * a[0],
			a[0] * b[1] - b[0] * a[1]
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ALG_HPP_