#ifndef _ML_ALG_HPP_
#define _ML_ALG_HPP_

#include <libmeme/Common.hpp>
#include <gcem/include/gcem.hpp>

#define _ML_UTIL ::ml::util::

#define ML_ASPECT(w, h)		((w != 0 && h != 0) ? ((float_t)w / (float_t)h) : 0.f)
#define ML_ASPECT2(v)		ML_ASPECT(v[0], v[1])
#define ML_MIN(lhs, rhs)	((lhs <= rhs) ? lhs : rhs)
#define ML_MAX(lhs, rhs)	((lhs >= rhs) ? lhs : rhs)
#define ML_CLAMP(v, lo, hi) ML_MIN(ML_MAX(v, lo), hi)

namespace ml::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_NODISCARD hash final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr auto fnv1a_basis{ static_cast<size_t>(14695981039346656037ULL) };

		static constexpr auto fnv1a_prime{ static_cast<size_t>(1099511628211ULL) };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> constexpr hash(Args && ... args) noexcept
			: m_value{ (*this)(std::forward<Args>(args)...) }
		{
		}

		constexpr hash() noexcept : m_value{ 0 } {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> constexpr size_t operator()(T const * arr, size_t size, size_t seed) noexcept
		{
			return (size > 0)
				? (*this)(arr + 1, size - 1, (seed ^ static_cast<size_t>(*arr)) * fnv1a_prime)
				: seed;
		}

		template <class T
		> constexpr size_t operator()(T const * arr, size_t size) noexcept
		{
			return (*this)(arr, size, fnv1a_basis);
		}

		template <class T, size_t N
		> constexpr size_t operator()(const T(&value)[N]) noexcept
		{
			return (*this)(value, N - 1);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Arr
		> constexpr size_t operator()(Arr const & value) noexcept
		{
			return (*this)(value.data(), value.size());
		}

		template <template <class, size_t...> class Arr, class T, size_t ... N
		> constexpr size_t operator()(Arr<T, N...> const & value) noexcept
		{
			return (*this)(value.data(), value.size());
		}

		template <template <class...> class Arr, class ... Ts
		> constexpr size_t operator()(Arr<Ts...> const & value) noexcept
		{
			return (*this)(value.data(), value.size());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr size_t operator()() const noexcept { return m_value; }

		constexpr operator size_t() const noexcept { return (*this)(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		size_t m_value;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> struct cast final
	{
		using type = typename T;

		template <class ... Args
		> ML_NODISCARD constexpr decltype(auto) operator()(Args && ... args) const noexcept
		{
			return static_cast<type>(std::forward<Args>(args)...);
		}
		
		static constexpr type minus_one		{ static_cast<type>( -1) };
		static constexpr type zero			{ static_cast<type>(  0) };
		static constexpr type one			{ static_cast<type>(  1) };
		static constexpr type two			{ static_cast<type>(  2) };
		static constexpr type three			{ static_cast<type>(  3) };
		static constexpr type four			{ static_cast<type>(  4) };
		static constexpr type five			{ static_cast<type>(  5) };
		static constexpr type six			{ static_cast<type>(  6) };
		static constexpr type seven			{ static_cast<type>(  7) };
		static constexpr type eight			{ static_cast<type>(  8) };
		static constexpr type nine			{ static_cast<type>(  9) };
		static constexpr type ten			{ static_cast<type>( 10) };
		static constexpr type fourty_five	{ static_cast<type>( 45) };
		static constexpr type sixty			{ static_cast<type>( 60) };
		static constexpr type ninety		{ static_cast<type>( 90) };
		static constexpr type one_hundred	{ static_cast<type>(100) };
		static constexpr type one_eighty	{ static_cast<type>(180) };
		static constexpr type three_sixty	{ static_cast<type>(360) };
		static constexpr type half			{ one / two };
		static constexpr type third			{ one / three };
		static constexpr type quarter		{ one / four };
		static constexpr type fifth			{ one / five };
		static constexpr type sixth			{ one / six };
		static constexpr type seventh		{ one / seven };
		static constexpr type eighth		{ one / eight };
		static constexpr type ninth			{ one / nine };
		static constexpr type tenth			{ one / ten };
		static constexpr type two_thirds	{ two / three };
		static constexpr type three_fourths	{ three / four };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class = std::enable_if<std::is_floating_point_v<T>>
	> struct pi final
	{
		using type = typename T;

		static constexpr auto value		{ cast<type>{}(3.14159265358979323846264338327L) };
		static constexpr auto two		{ value * cast<type>::two };
		static constexpr auto half		{ value * cast<type>::half };
		static constexpr auto quarter	{ value * cast<type>::quarter };
		static constexpr auto third		{ value * cast<type>::third };
		static constexpr auto deg2rad	{ value / cast<type>::one_eighty };
		static constexpr auto rad2deg	{ cast<type>::one_eighty / value };

		ML_NODISCARD constexpr operator type const() const { return value; }
	};

	template <class T
	> static constexpr auto pi_v{ pi<T>::value };
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace impl
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... T> struct sqrt;

		template <> struct sqrt<> final { sqrt() = delete; };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <> struct ML_NODISCARD sqrt<uintmax_t>
		{
			using type = typename uintmax_t;

			constexpr type operator()(type value) const
			{
				return sqrt<type>{}(cast<type>::one, cast<type>::three, value);
			}

			constexpr type operator()(type value, type curr, type prev) const
			{
				return (value <= prev)
					? sqrt<type>{}(value + curr, curr + cast<type>::two, prev)
					: (curr >> cast<type>::one) - cast<type>::one;
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <> struct ML_NODISCARD sqrt<float64_t>
		{
			using type = typename float64_t;

			constexpr type operator()(type value, type curr, type prev) const
			{
				return (curr == prev)
					? curr
					: sqrt<type>{}(value, cast<type>::half * (curr + value / curr), curr);
			}

			constexpr type operator()(type value) const
			{
				return (value >= cast<type>::zero && value < std::numeric_limits<type>::infinity())
					? sqrt<type>{}(value, value, cast<type>::zero)
					: std::numeric_limits<type>::quiet_NaN();
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class To, class From> struct ML_NODISCARD sqrt<To, From> : sqrt<From>
		{
			template <class U
			> constexpr To operator()(U const & value) const
			{
				return static_cast<To>(sqrt<From>{}(static_cast<From>(value)));
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <> struct ML_NODISCARD sqrt<intmax_t>
		{
			constexpr intmax_t operator()(intmax_t value) const
			{
				return sqrt<intmax_t, uintmax_t>{}(value);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <> struct ML_NODISCARD sqrt<float32_t>
		{
			constexpr float32_t operator()(float32_t value) const
			{
				return sqrt<float32_t, float64_t>{}(value);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	template <class T
	> struct ML_NODISCARD sqrt final : impl::sqrt<T>
	{
		constexpr T operator()(T value) const
		{
			return ::ml::util::impl::sqrt<T>{}(value);
		}
	};

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
		return ((*value) ? (1 + strlen(value + 1)) : 0);
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
		return ((sign(value) < (T)0)
			? (value * (T)-1)
			: value
		);
	}

	template <class T
	> ML_NODISCARD static constexpr T fact(T const & value)
	{
		return ((value > (T)1) ? value * fact(value - (T)1) : (T)1);
	}

	template <class Base, class Exp
	> ML_NODISCARD static constexpr Base pow(Base const & base, Exp const & exp)
	{
		using B = util::cast<Base>;
		using E = util::cast<Exp>;
		return ((exp < E::zero)
			? ((base == B::zero)
				? limits_t<Base>::nan
				: B::one / (base * pow(base, (-exp) - E::one)))
			: ((exp == E::zero)
				? B::one
				: ((exp == E::one)
					? base
					: base * pow(base, exp - E::one)
					)));
	}

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
				&& equals((lBegin + 1), lEnd, (rBegin + 1), rEnd))
			: ((lBegin == lEnd) && (rBegin == rEnd))
		);
	}

	template <class LI, class RI
	> ML_NODISCARD static constexpr bool less(LI lBegin, LI lEnd, RI rBegin, RI rEnd)
	{
		return ((lBegin != lEnd && rBegin != rEnd)
			? (((*lBegin) < (*rBegin))
				&& less((lBegin + 1), lEnd, (rBegin + 1), rEnd))
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
			value = value_at(index, arr);
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
		return sqrt<T>{}(sqr_magnitude<A, T, N...>(value));
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
		return (value / magnitude<A, T, N...>(value));
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
		const T det { determinant<M, T>(v) };
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