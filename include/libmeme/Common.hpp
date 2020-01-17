#ifndef _ML_COMMON_HPP_
#define _ML_COMMON_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if (ML_HAS_CXX11 && ML_HAS_CXX14)
#	include <cassert>
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
#	include <format>
#	include <ranges>
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if !defined(ML_ASSERT) && defined(assert)
#	define ML_ASSERT(expr) assert(expr)
#else
#	define ML_ASSERT(expr)
#endif

#define ML_ARGC			__argc
#define ML_ARGV			__argv
#define ML_WARGV		__wargv
#define ML_SERIALIZE	::std::ostream & operator <<
#define ML_DESERIALIZE	::std::istream & operator >>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace chrono	= _STD chrono;
	namespace pmr		= _STD pmr;
#if ML_HAS_CXX20
	namespace ranges	= _STD ranges;
	namespace views		= _STD ranges::views;
#endif

	using namespace _STD chrono_literals;
	using namespace _STD string_literals;
	using namespace _STD string_view_literals;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
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
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_COMMON_HPP_