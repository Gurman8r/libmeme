#ifndef _ML_COMMON_HPP_
#define _ML_COMMON_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if ML_HAS_CXX11 && ML_HAS_CXX14
#	include <cassert>
#	include <cctype>
#	include <chrono>
#	include <fstream>
#	include <functional>
#	include <iostream>
#	include <locale>
#	include <sstream>
#	include <type_traits>
#	include <utility>
#	include <vector>
#endif

#if ML_HAS_CXX17
#	include <any>
#	include <filesystem>
#	include <optional>
#	include <string_view>
#	include <variant>
#endif

#if ML_HAS_CXX20
#	include <concepts>
#	include <ranges>
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_ARGC			__argc
#define ML_ARGV			__argv
#define ML_WARGV		__wargv
#define ML_SERIALIZE	::std::ostream & operator <<
#define ML_DESERIALIZE	::std::istream & operator >>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using namespace ::std::chrono_literals;

	using namespace ::std::string_literals;

	using namespace ::std::string_view_literals;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_USING	int8_t		= typename ML_INT8;
	ML_USING	int16_t		= typename ML_INT16;
	ML_USING	int32_t		= typename ML_INT32;
	ML_USING	int64_t		= typename ML_INT64;

	ML_USING	uint8_t		= typename ML_UINT8;
	ML_USING	uint16_t	= typename ML_UINT16;
	ML_USING	uint32_t	= typename ML_UINT32;
	ML_USING	uint64_t	= typename ML_UINT64;

	ML_USING	float32_t	= typename ML_FLOAT32;
	ML_USING	float64_t	= typename ML_FLOAT64;
	ML_USING	float80_t	= typename ML_FLOAT80;

	ML_USING	intmax_t	= typename ML_INTMAX;
	ML_USING	uintmax_t	= typename ML_UINTMAX;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_USING	byte_t		= typename uint8_t;
	ML_USING	float_t		= typename float32_t;
	ML_USING	intptr_t	= typename intmax_t;
	ML_USING	ptrdiff_t	= typename intptr_t;
	ML_USING	size_t		= typename uintmax_t;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_USING	C_string	= typename char const *;
	ML_USING	C_wstring	= typename wchar_t const *;
	ML_USING	C_u16string	= typename char16_t const *;
	ML_USING	C_u32string	= typename char32_t const *;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <intmax_t Num, intmax_t Den = 1
	> ML_USING ratio_t	= typename ::std::ratio<Num, Den>;
	ML_USING atto_t		= typename ratio_t<1LL, 1000000000000000000LL>;
	ML_USING femto_t	= typename ratio_t<1LL, 1000000000000000LL>;
	ML_USING pico_t		= typename ratio_t<1LL, 1000000000000LL>;
	ML_USING nano_t		= typename ratio_t<1LL, 1000000000LL>;
	ML_USING micro_t	= typename ratio_t<1LL, 1000000LL>;
	ML_USING milli_t	= typename ratio_t<1LL, 1000LL>;
	ML_USING centi_t	= typename ratio_t<1LL, 100LL>;
	ML_USING deci_t		= typename ratio_t<1LL, 10LL>;
	ML_USING deca_t		= typename ratio_t<10LL, 1LL>;
	ML_USING hecto_t	= typename ratio_t<100LL, 1LL>;
	ML_USING kilo_t		= typename ratio_t<1000LL, 1LL>;
	ML_USING mega_t		= typename ratio_t<1000000LL, 1LL>;
	ML_USING giga_t		= typename ratio_t<1000000000LL, 1LL>;
	ML_USING tera_t		= typename ratio_t<100000000000LL, 1LL>;
	ML_USING peta_t		= typename ratio_t<1000000000000000LL, 1LL>;
	ML_USING exa_t		= typename ratio_t<1000000000000000000LL, 1LL>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_NODISCARD hashof final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr auto fnv1a_basis{ static_cast<size_t>(14695981039346656037ULL) };

		static constexpr auto fnv1a_prime{ static_cast<size_t>(1099511628211ULL) };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args
		> constexpr hashof(Args && ... args) noexcept
			: m_value{ (*this)(std::forward<Args>(args)...) }
		{
		}

		constexpr hashof() noexcept : m_value{ 0 } {}

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
	> struct cast_t final
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

	template <class ... T> struct limits_t;

	template <class T
	> struct limits_t<T> final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using type = typename T;

		static constexpr type infinity		{ std::numeric_limits<type>::infinity() };
		static constexpr type nan			{ std::numeric_limits<type>::quiet_NaN() };
		static constexpr type min			{ std::numeric_limits<type>::min() };
		static constexpr type max			{ std::numeric_limits<type>::max() };
		static constexpr type epsilon		{ std::numeric_limits<type>::epsilon() };
		static constexpr type half_epsilon	{ epsilon * cast_t<type>::half };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> struct pi_t final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using type = typename T;

		static_assert(std::is_floating_point_v<type>);

		static constexpr auto value		{ cast_t<type>{}(3.14159265358979323846264338327L) };
		static constexpr auto two		{ value * cast_t<type>::two };
		static constexpr auto half		{ value * cast_t<type>::half };
		static constexpr auto quarter	{ value * cast_t<type>::quarter };
		static constexpr auto third		{ value * cast_t<type>::third };
		static constexpr auto deg2rad	{ value / cast_t<type>::one_eighty };
		static constexpr auto rad2deg	{ cast_t<type>::one_eighty / value };

		ML_NODISCARD constexpr operator type const &() const { return value; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> struct pack_t final
	{
		template <template <class...> class T
		> struct apply
		{
			using type = typename T<Args...>;
		};
	};

	namespace impl
	{
		template <
			template <class...> class To,
			template <template <class...> class> class T
		> struct unpack final
		{
			using type = typename T<To>::type;
		};
	}

	template <
		template <class...> class To,
		template <template <class...> class> class T
	> using unpack_t = typename impl::unpack<To, T>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_COMMON_HPP_